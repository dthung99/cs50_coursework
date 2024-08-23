#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string text = get_string("Text: ");
    int len = strlen(text);
    int number_of_letter = 0;
    int number_of_word = 1;
    int number_of_sentence = 0;
    for (int i = 0; i < len; i++)
    {
        if (isalpha(text[i]))
        {
            number_of_letter++;
        }
        if (text[i] == ' ')
        {
            number_of_word++;
        }
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            number_of_sentence++;
        }
    }
    float index = 0.0588 * (number_of_letter * 100.0) / number_of_word -
                  0.296 * (number_of_sentence * 100.0) / number_of_word - 15.8;
    int result = round(index);
    if (result < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (result > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", result);
    }
}
