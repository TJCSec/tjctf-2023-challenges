#include <stdio.h>
#include <string.h>

char *cow =
    "< %s >\n"
    "        \\   ^__^\n"
    "         \\  (oo)\\_______\n"
    "            (__)\\       )\\/\\\n"
    "                ||----w |\n"
    "                ||     ||\n";

char *flag = "8.'8*{;8m33[o[3[3[%\")#*\\}";

int main()
{
    char text[32], raw[32];
    printf("Enter text: ");

    fgets(text, 32, stdin);

    text[strlen(text) - 1] = '\0';

    strcpy(raw, text);

    int len = strlen(text);

    for (int i = 0; i < len; i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            text[i] -= 60;
        }
        else if (text[i] >= 'A' && text[i] <= 'Z')
        {
            text[i] += 32;
        }
        else if (text[i] >= '0' && text[i] <= '4')
        {
            text[i] += 43;
        }
        else if (text[i] >= '5' && text[i] <= '9')
        {
            text[i] -= 21;
        }
        else if (text[i] != '{' && text[i] != '}')
        {
            puts("wtmoo is this guess???");
            printf("%c\n", text[i]);
            return 1;
        }
    }

    if (strcmp(text, flag) != 0)
    {
        printf(cow, text);
    }
    else
    {
        printf(cow, raw);
    }

    return 0;
}
