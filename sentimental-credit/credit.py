import cs50


def sum_digit(num):
    num = str(num)
    total = 0
    for i in num:
        total += int(i)
    return total


def main():
    # get user input
    card_number = cs50.get_string("Input card number: ")

    # Check card length
    card_type = "INVALID"
    if len(card_number) not in [13, 15, 16]:
        print(card_type)
        return

    # Check the start of the card
    if card_number[0:2] in ["34", "37"]:
        card_type = "AMEX"
    elif card_number[0:2] in ["51", "52", "53", "54", "55"]:
        card_type = "MASTERCARD"
    elif card_number[0] == "4":
        card_type = "VISA"
    else:
        print(card_type)
        return
    sum = 0
    # Perform Luhn Algorithm
    for i, num in enumerate(card_number[::-1]):
        if int(i) % 2 == 1:
            sum += sum_digit(int(num)*2)
        else:
            sum += int(num)
    if sum % 10 == 0:
        print(card_type)
        return

    card_type = "INVALID"
    print(card_type)
    return


if __name__ == "__main__":
    main()
