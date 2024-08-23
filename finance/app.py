import os

from cs50 import SQL
import datetime
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get current stock
    total_money = 0
    stock_datas = db.execute(
        "SELECT stock_type, SUM(stock_amount) AS total_stock_amount FROM user_stocks_database WHERE user_id=? GROUP BY stock_type;", session["user_id"])
    for stock_data in stock_datas:
        stock_data["stock_price_current"] = (lookup(stock_data["stock_type"]))["price"]
        stock_data["total_price_current"] = stock_data["stock_price_current"] * \
            stock_data["total_stock_amount"]
        total_money += stock_data["total_price_current"]
    # Get current cash
    cash = db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"]
    total_money += cash
    return render_template("/index.html", stock_datas=stock_datas, cash=round(cash, 4), total_money=round(total_money, 4))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Find the stock data
        stock_data = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")
        if stock_data is None:
            return apology("Stock do not exist")
        # Check if the shares is a positive number
        if not (shares.isdigit() and int(shares) > 0):
            return apology("Shares must be positive integer")
        # Get user money and compare it to the cost
        id = session["user_id"]
        user_money = db.execute("SELECT cash FROM users WHERE id=?", id)
        user_money = user_money[0]["cash"]
        # Total cost
        total_cost = float(stock_data["price"])*int(shares)
        if total_cost > user_money:
            return apology("You don't have enough money")
        # Update user money
        db.execute("UPDATE users SET cash=? WHERE id=?", user_money-total_cost, id)
        # Update user stocks
        current_time = datetime.datetime.now()
        sql_datetime = current_time.strftime('%Y-%m-%d %H:%M:%S')
        db.execute("""
                   INSERT INTO user_stocks_database (user_id, order_date_time, stock_type, stock_amount, stock_price_at_buy)
                   VALUES (?, ?, ?, ?, ?);
                   """,
                   id,
                   sql_datetime,
                   stock_data["symbol"],
                   shares,
                   float(stock_data["price"]))
        return redirect("/")
    return render_template("/buy.html")


@app.route("/update_stock_in_buy", methods=["GET", "POST"])
@login_required
def update_stock_in_buy():
    """Update the buy page with AJAX"""
    if request.method == "POST":
        # Find the stock data and send for AJAX update
        stock_data = lookup(request.form.get("symbol"))
    return jsonify(stock_data)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get current stock
    stock_datas = db.execute(
        "SELECT * FROM user_stocks_database WHERE user_id=?;", session["user_id"])
    # for stock_data in stock_datas:
    #     stock_data["stock_price_current"]=(lookup(stock_data["stock_type"]))["price"]
    #     stock_data["total_price_current"]=stock_data["stock_price_current"]*stock_data["total_stock_amount"]
    #     total_money+=stock_data["total_price_current"]
    print(stock_datas)
    return render_template("/history.html", stock_datas=stock_datas)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Find the stock data and redirect if found
        stock_data = lookup(request.form.get("symbol"))
        if not stock_data:
            return apology("No such stocks available!", 400)
        else:
            return render_template("quoted.html", stock_data=stock_data)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)
        # Ensure password was submitted
        if not password:
            return apology("must provide password", 400)
        # Ensure confirmation equal password
        if not confirmation == password:
            return apology("confirmation must equal password", 400)
        # Check for duplicate username and insert into the db if not duplicate
        try:
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)",
                username,
                generate_password_hash(password)
            )
            return redirect("/login")
        except ValueError:
            return apology("Username already exists", 400)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Find the stock data
        symbol = request.form.get("symbol")
        stock_data = lookup(symbol)
        symbol = stock_data["symbol"]
        shares = request.form.get("shares")
        if stock_data is None:
            return apology("Stock do not exist", 400)
        # Check if the shares is a positive number
        if not (shares.isdigit() and int(shares) > 0):
            return apology("Shares must be positive integer", 400)
        # Get user total shares of that stock and compare it to the sell ammount
        id = session["user_id"]
        user_shares = db.execute(
            "SELECT SUM(stock_amount) AS total_stock_amount FROM user_stocks_database WHERE user_id=? AND stock_type=? GROUP BY stock_type", id, symbol)
        total_stock_amount = user_shares[0]["total_stock_amount"] if len(user_shares) > 0 else 0

        shares = int(shares)
        if total_stock_amount < shares:
            return apology("You don't have enough shares", 400)
        # Update user money
        # Total gain
        total_gain = float(stock_data["price"])*shares
        # Get user money
        user_money = db.execute("SELECT cash FROM users WHERE id=?", id)[0]["cash"]
        # Update the money
        db.execute("UPDATE users SET cash=? WHERE id=?", user_money+total_gain, id)
        # Update user stocks
        current_time = datetime.datetime.now()
        sql_datetime = current_time.strftime('%Y-%m-%d %H:%M:%S')
        db.execute("""
                   INSERT INTO user_stocks_database (user_id, order_date_time, stock_type, stock_amount, stock_price_at_buy)
                   VALUES (?, ?, ?, ?, ?);
                   """,
                   id,
                   sql_datetime,
                   stock_data["symbol"],
                   -shares,
                   float(stock_data["price"]))
        return redirect("/")
    # Get the current available stocks
    possessed_stocks = db.execute(
        "SELECT DISTINCT stock_type FROM user_stocks_database WHERE user_id=?", session["user_id"])
    return render_template("/sell.html", possessed_stocks=possessed_stocks)
