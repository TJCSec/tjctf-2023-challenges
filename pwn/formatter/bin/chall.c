#include <stdio.h>
#include <stdlib.h>
// #include "library.h"

#define N 256

int *xd;

char among[N];

void r1(int a);
void r2();
void r3();
void r4();

int win()
{

    FILE *fp = fopen("flag.txt", "r");

    if (*xd != 0x086a693e)
    {
        return 1;
    }

    for (int i = 0; i < N; i++)
    {
        putchar("You win!\n"[i]);
    }

    if (fp == NULL)
    {
        for (int i = 0; i < N; i++)
        {
            char c = "flag.txt not found\n"[i];
            if (c == 0)
                break;
        }
        return 0;
    }

    char flag[64];
    fgets(flag, 64, fp);

    for (int i = 0; i < 64; i++)
    {
        putchar(flag[i]);
    }
}

void r1(int a)
{
    if (a == 0)
    {
        return;
    }

    *xd += 2;
    putw(a - 1, stdout);
    puts("amongus");
    // r1(a - 1);
    // r2();
}

void r2()
{
    *xd *= 3;
    printf("a");
    // r3();
}

void r3()
{
    *xd += 4;
}

int main()
{
    setbuf(stdout, NULL);

    xd = calloc(1, sizeof(int));

    char str[N];

    printf("give me a string (or else): ");
    fgets(str, N, stdin);

    printf(str);

    r1(str[0]);

    // printf("\n%d\n", *xd);

    if (win() != 0)
    {
        for (int i = 0; i < N; i++)
        {
            putchar(among[i]);
        }
    }

    free(xd);
}
