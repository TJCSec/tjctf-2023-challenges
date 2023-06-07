#include <stdio.h>
#include <string.h>

const char flag[] = "\022\021\000\025\vH<\022\fD\000\020Q\031.\026\003\034B\021\nJrV\rztO";

#define N 64

int main(){
    char guess[N];

    puts("Enter your flag");

    fgets(guess, N, stdin);

    if (strlen(guess) - 1 != 32){
        puts("bad");
        return 1;
    }
    for (int i = 4; i < strlen(guess) - 1; i++){
        if ((guess[i] ^ guess[i-4]) != flag[i-4]){
            puts("you're def wrong smh");
            return 1;
        }
    }
    puts("you might be right??? you might be wrong.... who knows?");
    return 0;
}
