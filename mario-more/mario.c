#include <cs50.h>
#include <stdio.h>

void showblock(int n);

int main(void)
{
    int n;
    do
    {
        n = get_int("Enter height of pyramid: ");
    }
    while (n < 1 || n > 8);

    showblock(n);
}

void showblock(int n)
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            printf(" ");
        }
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
