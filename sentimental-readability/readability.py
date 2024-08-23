import cs50


def main():
    # get user text
    text = cs50.get_string("Your text: ")
    # Perform Coleman-Liau index
    L = len([char for char in text if char.isalpha()])
    S = len([char for char in text if char in [".", "!", "?"]])
    W = len([char for char in text if char == " "]) + 1
    L = L / W * 100
    S = S / W * 100
    score = round(0.0588 * L - 0.296 * S - 15.8)
    if score < 1:
        print("Before Grade 1")
    elif score >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {score}")
    return


if __name__ == "__main__":
    main()
