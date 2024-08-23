#include <cs50.h>
#include <stdio.h>
#include <string.h>

string get_card_type(void);
int ctoi(char c);
int sum_of_digit(int x);
string card_type = "INVALID\n";

int main(void)
{
    string card_number = get_card_type();
    int len_card = strlen(card_number);
    int sum_even = 0;
    // Using Luhn’s Algorithm to check validity
    for (int i = len_card - 2; i >= 0; i = i - 2)
    {
        sum_even += sum_of_digit(ctoi(card_number[i]) * 2);
    }
    for (int i = len_card - 1; i >= 0; i = i - 2)
    {
        sum_even += ctoi(card_number[i]);
    }
    if (sum_even % 10 != 0)
    {
        card_type = "INVALID\n";
    }
    printf("%s", card_type);
}

string get_card_type(void)
{
    // Return the type of card
    string card_number = get_string("Input your card number: ");
    switch (strlen(card_number))
    {
        case 15:
            if (card_number[0] == '3' && (card_number[1] == '4' || card_number[1] == '7'))
            {
                card_type = "AMEX\n";
            }
            break;
        case 16:
            if (card_number[0] == '4')
            {
                card_type = "VISA\n";
            }
            if (card_number[0] == '5' && ctoi(card_number[1]) > 0 && ctoi(card_number[1]) < 6)
            {
                card_type = "MASTERCARD\n";
            }
            break;
        case 13:
            if (card_number[0] == '4')
            {
                card_type = "VISA\n";
            }
            break;
        default:
            card_type = "INVALID\n";
    }
    // "" or "MASTERCARD\n" or "" or "",
    return card_number;
}

int ctoi(char c)
{
    return c - '0';
};

int sum_of_digit(int x)
{
    // Sum of digit of variable 0<=x<=99, return -1 if x is outside this range
    if (0 <= x && x <= 9)
    {
        return x;
    }
    else if (x <= 99)
    {
        return x % 10 + x / 10;
    }
    else
    {
        return -1;
    }
};
