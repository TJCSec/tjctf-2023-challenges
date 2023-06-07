#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "game.h"

#define STDOUT_BUF_SIZE 4096
char stdout_buf[STDOUT_BUF_SIZE];

char player_tile = '@';

int main(void) {

    player_t p;
    map_t map;

    setup_terminal();
    setvbuf(stdout, stdout_buf, _IOFBF, STDOUT_BUF_SIZE);

    init_player(&p);
    init_map(map, &p);
    print_map(map);
    signal(SIGINT, sigint_handler);
    while (p.r != HEIGHT - 1 || p.c != WIDTH - 1) {
        move_player(&p, getchar(), map);
        print_map(map);
    }
    puts("You win!");
}

void win(void) {
    system("/bin/sh");
}

struct termios tty_old;

void setup_terminal(void)
{
    struct termios tty;

    if (tcgetattr(STDIN_FILENO, &tty_old) == -1) {
        return;
    }

    tty = tty_old;
    tty.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON);
    tty.c_lflag &= ~(ECHO | ICANON | IEXTEN);
    tty.c_cflag |= (CS8);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
    atexit(teardown_terminal);
}

void teardown_terminal(void)
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tty_old) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void sigint_handler(int sig) {
    exit(EXIT_SUCCESS);
}

void init_player(player_t *p) {
    p->r = 4;
    p->c = 4;
}

void init_map(map_t map, player_t *p) {
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (r == HEIGHT - 1 && c == WIDTH - 1) {
                map[r][c] = 'X';
            } else if (r == p->r && c == p->c) {
                map[r][c] = player_tile;
            } else {
                map[r][c] = '.';
            }
        }
    }
}

void clear_screen(void) {
    fputs("\x1b[H\x1b[2J\x1b[3J", stdout);
}

void find_player_pos(map_t map) {
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (map[r][c] == player_tile) {
                printf("Player position: %d %d\n", r, c);
                return;
            }
        }
    }
}

void find_end_tile_pos(map_t map) {
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (map[r][c] == 'X') {
                printf("End tile position: %d %d\n", r, c);
                return;
            }
        }
    }
}

void print_map(map_t map) {
    clear_screen();
    find_player_pos(map);
    find_end_tile_pos(map);
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            putchar(map[r][c]);
        }
        putchar('\n');
    }
    fflush(stdout);
}

void move_player(player_t *p, char c, map_t map) {
    if (c == 'l') {
        player_tile = getchar();
        return;
    }
    map[p->r][p->c] = '.';
    if (c == 'w') {
        p->r--;
    } else if (c == 's') {
        p->r++;
    } else if (c == 'a') {
        p->c--;
    } else if (c == 'd') {
        p->c++;
    }
    map[p->r][p->c] = player_tile;
}
