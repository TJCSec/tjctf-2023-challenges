#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define N 256
#define DELAY 25

void type_text(char *str)
{
    int n = strlen(str);
    for (int i = 0; i < n; i++)
    {
        printf("%c", str[i]);
        usleep(DELAY * 1000);
    }
}

void prompt(char *str, char *resp, int n)
{
    type_text(str);
    fgets(resp, n, stdin);

    resp[strlen(resp) - 1] = '\0';
}

int main()
{
    setbuf(stdout, NULL);

    type_text("last night, I had a dream...\ntaylor sw1ft, the dollar store version, appeared!\n");

    char resp[N];
    prompt("what should I do? ", resp, N);

    if (strcmp("sing", resp) != 0)
    {
        puts("no, no, that's a bad idea.");
        exit(0);
    }

    prompt("that's a great idea!\nI started to sing the following lyrics: ", resp, N);

    if (strcmp("maybe I asked for too [many challenges to be written]", resp) != 0)
    {
        puts("no, that's a dumb lyric.");
        exit(0);
    }

    type_text("ok... that's a weird lyric but whatever\n");
    prompt("that leads me to ask... how many challenges did you ask for??? ", resp, N);

    volatile unsigned long n1, n2;
    n1 = atol(resp);

    if ((((n1 * 3) ^ 47342 - 534) % 1313) * 35 % 1515 != 1370)
    {
        type_text("that's a stupid number.\n");
        exit(0);
    }

    prompt("ok yeah you're asking too much of everyone; try to lower the number??? ", resp, N);
    n2 = atol(resp);

    if ((((n2 * 5) % 7777 | 1111) * 35 - 5) % 1000 != 80)
    {
        type_text("yeah.");
        exit(0);
    }

    if (n1 % n2 != 20202020 || n1 * n2 != 14590347874298541)
    {
        type_text("ok but they might think that's too much comparatively, duh.\n");
        exit(0);
    }

    type_text("that's a lot more reasonable - good on you!\n");

    usleep(DELAY * 3 * 1000);

    type_text("ok, now that we've got that out of the way, back to the story...\n");

    type_text("taylor was like, \"wow, you're so cool!\", and I said, \"no, you're so cool!\"\n");

    type_text("after that, we kinda just sat in silence for a little bit. I could kinda tell I was losing her attention, so ");

    int asked = 0;

    for (int i = 0; i < 12; i++)
    {
        prompt("what should I do next? ", resp, N);
        if (strcmp("ask her about some flags", resp) == 0)
        {
            asked++;
        }
        else if (strcmp("ask her about her new album", resp) == 0)
        {
            asked *= asked;
        }
        else if (strcmp("ask her about her tour", resp) == 0)
        {
            asked += 22;
        }
        else
        {
            type_text("no, that's weird\n");
            exit(0);
        }
    }

    if (asked != 2351)
    {
        type_text("taylor died of boredom\n");
        exit(0);
    }

    type_text("taylor got sick and tired of me asking her about various topics, so she finally responded: ");

    FILE *f = fopen("flag.txt", "r");

    if (f == NULL)
    {
        type_text("no flag </3\n");
        exit(0);
    }

    char flag[N];
    fgets(flag, N, f);

    type_text(flag);
}
