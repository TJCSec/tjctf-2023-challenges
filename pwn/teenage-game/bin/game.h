#ifndef GAME_H
#define GAME_H

#define WIDTH 90
#define HEIGHT 30

typedef struct player {
    int r;
    int c;
} player_t;

typedef char map_t[HEIGHT][WIDTH];

void win(void);
void setup_terminal(void);
void teardown_terminal(void);
void sigint_handler(int sig);
void init_player(player_t *p);
void init_map(map_t map, player_t *p);
void clear_screen(void);
void find_player_pos(map_t map);
void find_end_tile_pos(map_t map);
void print_map(map_t map);
void move_player(player_t *p, char c, map_t map);

#endif
