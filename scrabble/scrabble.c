#include <cs50.h>
#include <stdio.h>
#include <string.h>

int score_list[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
int convert_upper_to_lower = 'a' - 'A';

int getscore(char c);

int main(void)
{
    string word1 = get_string("Player 1 ");
    string word2 = get_string("Player 2 ");
    int len1 = strlen(word1);
    int p1_score = 0;
    for (int i = 0; i < len1; i++)
    {
        p1_score += getscore(word1[i]);
    }
    int len2 = strlen(word2);
    int p2_score = 0;
    for (int i = 0; i < len2; i++)
    {
        p2_score += getscore(word2[i]);
    }
    if (p1_score > p2_score)
    {
        printf("Player 1 wins!\n");
    }
    else if (p1_score < p2_score)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int getscore(char c)
{
    if (c >= 65 && c <= 90)
    {
        c = c + convert_upper_to_lower;
    }
    if (c < 97 || c > 122)
    {
        return 0;
    }
    return score_list[c - 'a'];
}
