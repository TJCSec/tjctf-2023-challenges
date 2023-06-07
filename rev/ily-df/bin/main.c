#include <ncurses.h>
#include "stdio.h"
#include "time.h"
#include "unistd.h"     //STDIN_FILENO
#include "craft.h"
#include "world.h"

#define SCREEN_WIDTH 96
#define SCREEN_HEIGHT 25

#define BREAK_MODE 'b'
#define CRAFT_MODE 'c'
#define PLACE_MODE 'p'
char mode = PLACE_MODE;
char to_be_placed = 'L';
int placectr = -1;
int placingX = -1;
int placingY = -1;
int placingZ = -1;



struct vec2
{
    float x;
    float y;
};

float dot(struct vec2 a, struct vec2 b)
{
    return a.x * b.x + a.y * b.y;
}


struct Astro
{
    int x;
    int y;
    int z;
    int inv[256];
};

int breakingX = -1;
int breakingY = -1;
int breakingZ = -1;
int breakctr = -1;
#define breakTime 5

struct Astro astro;


int tile_passable(char* t)
{
    return *t == AIR || *t == PICK;
}

int tile_can_break(char* t, int pick)
{
	if (!tile_passable(t))
		return pick;
	return 1;	
}

int tile_break_time(char* t)
{
	if (*t == AIR)
		return 0;
	if (*t == ROCK)
		return 5;
	if (*t == SOIL)
		return 1;
	if (*t == WALL)
		return 10;
	if (*t == IRON_ORE)
		return 7;
	if (*t == TIN_ORE)
		return 3;
	if (*t == COPPER_ORE)
		return 4;
	if (*t == IRON)
		return 14;
	if (*t == TIN)
		return 6;
	if (*t == COPPER)
		return 8;
	if (*t == SAND)
		return 2;
	return 1;
}

void makeTile(char* t, char chr)
{
    *t = chr;
}

int test_air(int x, int y, int z)
{
    if (x < 0 || x > WORLD_LEN - 1 || y < 0 || y > WORLD_HEIGHT - 1 || z < 0 || z > WORLD_LEN - 1)
        return 0;
    return world[x][y][z] == AIR;
}

int should_render(int x, int y, int z)
{
    int verd = 0;
    verd |= test_air(x - 1, y, z);
    verd |= test_air(x + 1, y, z);
    
    verd |= test_air(x, y - 1, z);
    verd |= test_air(x, y + 1, z);
    
    verd |= test_air(x, y, z - 1);
    verd |= test_air(x, y, z + 1);

    return verd;
}

void printWorld()
{
    erase();
    if (mode != PLACE_MODE)
        printw("Mode: %c\n", mode);
    else
        printw("Mode: %c\nTo be placed: %c\n", mode, to_be_placed);

    for (int i = 0; i < SCREEN_WIDTH + 1; i++)
        printw("-");
    printw("\n");
    int cornerX = astro.x - SCREEN_HEIGHT / 2;
    if (cornerX < 0)
        cornerX = 0;
    for (int x = cornerX; (x < SCREEN_HEIGHT + cornerX) && (x < WORLD_LEN); x++)
    {
        printw("|");
        int cornerZ = astro.z - SCREEN_WIDTH/2;
        if (cornerZ < 0)
            cornerZ = 0;
        int z = cornerZ;
        char* c1 = 0;
        char* c2 = 0;
        for (; (z < SCREEN_WIDTH + cornerZ) && (z < WORLD_LEN); z++)
        {
            char thisLchar = world[x][astro.y][z];
            if (thisLchar == AIR && astro.y > 0)
            {
                char nextLchar = world[x][astro.y - 1][z];
                if (nextLchar != AIR) {
                    if (nextLchar != ROCK)
                        thisLchar = nextLchar + 32; //make lowercase
                    else
                        thisLchar = ',';
                }
                else
                {
                    if (astro.y > 1)
                    {
                        nextLchar = world[x][astro.y - 2][z];
                    }
                    if (nextLchar != AIR)
                    {
                        thisLchar = '.';
                    }
                }
            }
            
            if (!tile_passable(&world[x][astro.y][z]) && astro.y < WORLD_HEIGHT - 1 && tile_passable(&world[x][astro.y + 1][z]))
            {
                int verd = 0;
                verd |= test_air(x - 1, astro.y, z);
                verd |= test_air(x + 1, astro.y, z);

                verd |= test_air(x, astro.y, z - 1);
                verd |= test_air(x, astro.y, z + 1);
                if (verd)
                    thisLchar = '^';
            }

            if (!should_render(x,astro.y,z))
              thisLchar = AIR;


            if (x == astro.x)
            {
                if (z == astro.z)
                    thisLchar = 'A';
            }
            if (!(c2) && !(c1))
            {
                printw("%c", thisLchar);
            }
            else
            {
                printw("%s%c%s", c1, thisLchar, c2);
            }

        }
        //z++
        printw("|\n");
    }
    for (int i = 0; i < SCREEN_WIDTH + 1; i++)
        printw("-");
    printw("\nx, y, z: %i, %i, %i\n", astro.x, astro.y, astro.z);
    
    //C++ HAHAHAHAHHAHAHAH
    printw("Inventory:\n");
    for (int c = 0; c < 255; c++)
    {
        if (astro.inv[c] > 0)
            printw("'%c': %i   ", c, astro.inv[c]);
    }


    if (mode == CRAFT_MODE)
    {
    	printw("\nAvailible Recipes:\n");
    	for (int i = 0; i < numRecipes; i++)
    	{
    		if (recipe_can_craft(&recipes[i], astro.inv))
    		{
	    		printw("'%c' (%s) from ", recipes[i].out,recipes[i].namestr);
	    		for (int j = 0; j < recipes[i].numIngredients; j++)
	    		{
	    			printw("'%c', ",recipes[i].ingredients[j]);
	    		}
	    		printw("\n");
    		}
    	}
    }

    printw("\n\n\n\n\n");
    refresh();

    //delay 10 ms
    struct timespec ts;

    int msec = 10;
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    //nanosleep(&ts, &ts);
}

void doKeyboard()
{
    int m = -1;
    int i = 0;
    while (1)
    {
        char c = getch();
        if (c == -1)
            break;
        if (i == 0)
            m = c;
        i++;
        if (i == 50)
            break;
    }
    //printf("Msg: %s\n",message);
    //sleep(1);
    if (m < 0)
        return;


    //printf("%c\n",message[i]);
    char curr = m;

    int oldX = astro.x;
    int oldY = astro.y;
    int oldZ = astro.z;

    if (curr == BREAK_MODE)
        mode = BREAK_MODE;
    if (curr == PLACE_MODE)
        mode = PLACE_MODE;
    if (curr == CRAFT_MODE)
    	mode = CRAFT_MODE;
        
    if (curr == 'w')
        {
            astro.x--;
            if (astro.x < 0)
                astro.x = 0;
        }
    if (curr == 's')
        {
            astro.x++;
            if (astro.x >= WORLD_LEN)
                astro.x = WORLD_LEN - 1;
        }
    if (curr == 'a')
        {
            astro.z--;
            if (astro.z < 0)
                astro.z = 0;
        }
    if (curr == 'd')
        {
            astro.z++;
            if (astro.z >= WORLD_LEN)
                astro.z = WORLD_LEN - 1;
        }

    //Can't go thru this...?
    if (!tile_passable(&world[astro.x][astro.y][astro.z]))
    {

        if ((astro.y + 1 < WORLD_HEIGHT - 1) && tile_passable(&world[astro.x][astro.y + 1][astro.z]))
        {
            astro.y++;
        }
        else
        {
            astro.x = oldX;
            astro.y = oldY;
            astro.z = oldZ;
        }
    }

    if (mode == BREAK_MODE)
    {
    	int d = 0;
        if (curr == 'i')
        {
            breakingX = astro.x - 1;
            d = 1;
        }
        if (curr == 'k')
        {
            breakingX = astro.x + 1;
            d = 1;
        }
        if (curr == 'j')
        {
            breakingZ = astro.z - 1;
            d = 1;
        }
        if (curr == 'l')
        {
            breakingZ = astro.z + 1;
            d = 1;
            }
        if (curr == 'u')
        {
            breakingY = astro.y + 1;
            d = 1;
        }
        if (curr == 'm')
        {
            breakingY = astro.y - 1;
            d = 1;
        }
        if (d)
        {
     	    
            if (breakingX < 0)
                breakingX = astro.x;
            if (breakingY < 0)
                breakingY = astro.y;
            if (breakingZ < 0)
                breakingZ = astro.z;
            breakctr = tile_break_time(&world[breakingX][breakingY][breakingZ]);
        }
    }

    if (mode == PLACE_MODE)
    {
        if (curr == 'w' || curr == 'a' || curr == 's' || curr == 'd' || curr == 'p' || curr == 'b' || curr == CRAFT_MODE)
        {
            return;
        }

        if (curr == 'i' || curr == 'u' || curr == 'j' || curr == 'm' || curr == 'k' || curr == 'l' || curr == 'h')
        {
            if (curr == 'i')
            {
                placingX = astro.x - 1;
                placectr = breakTime;
            }
            if (curr == 'k')
            {
                placingX = astro.x + 1;
                placectr = breakTime;
            }
            if (curr == 'j')
            {
                placingZ = astro.z - 1;
                placectr = breakTime;
            }
            if (curr == 'l')
            {
                placingZ = astro.z + 1;
                placectr = breakTime;
            }
            if (curr == 'u')
            {
                placingY = astro.y + 1;
                placectr = breakTime;
            }
            if (curr == 'm')
            {
                placingY = astro.y - 1;
                placectr = breakTime;
            }
            if (breakTime == placectr)
            {
                if (placingX < 0)
                    placingX = astro.x;
                if (placingY < 0)
                    placingY = astro.y;
                if (placingZ < 0)
                    placingZ = astro.z;
            }
        }
        else //this is setting a block to place
        {
            to_be_placed = curr;
        }
    }
    if (mode == CRAFT_MODE)
    {
    	 if ( ! (curr == 'w' || curr == 'a' || curr == 's' || curr == 'd' || curr == 'p' || curr == 'b' || curr == CRAFT_MODE || curr == PLACE_MODE || curr == BREAK_MODE || curr == 'i' || curr == 'u' || curr == 'j' || curr == 'm' || curr == 'k' || curr == 'l'))
    	 {
    	 
    	 	for (int i = 0; i < numRecipes; i++)
    	 	{
    	 		if (recipes[i].out == curr) 		
 	   	 		recipe_craft(&recipes[i], astro.inv);
    	 	}
    	 }
    }
}

void drop_sand()
{
	for (int x = 0; x < WORLD_LEN; x++)
    	{
        for (int z = 0; z < WORLD_LEN; z++)
        {
            for (int y = 1; y < WORLD_HEIGHT; y++)
            {
			if (world[x][y][z] == SAND)
			{
				if (world[x][y-1][z] == AIR)
				{
					world[x][y-1][z] = SAND;
					world[x][y][z] = AIR;
				}
			}
		}
		}
	}
}

void recur_smash(int breakingX, int breakingY, int breakingZ)
{
	if (breakingX < 0 || breakingX > WORLD_LEN -1 || breakingY < 0 || breakingY > WORLD_HEIGHT -1 ||breakingZ < 0 || breakingZ > WORLD_LEN -1)
		return;
	if (breakingZ > astro.z + 40 || breakingZ < astro.z-40)
		return; 
	if (world[breakingX][breakingY][breakingZ] != ROCK)
	{
		return;
	} 
	
	world[breakingX][breakingY][breakingZ] = AIR;
	
	recur_smash(breakingX+1, breakingY, breakingZ);
	recur_smash(breakingX-1, breakingY, breakingZ);
	recur_smash(breakingX, breakingY+1, breakingZ);
	recur_smash(breakingX, breakingY-1, breakingZ);
	recur_smash(breakingX, breakingY, breakingZ+1);
	recur_smash(breakingX, breakingY, breakingZ-1);
}

void astro_timeUpdate()
{
    int x = astro.x;
    int y = astro.y;
    int z = astro.z;

    //test falling
    if (y > 0)
    {
        if (tile_passable(&world[x][y][z]) && tile_passable(&world[x][y - 1][z]))
        {
            (astro.y)--; //fall
        }
    }

    if (breakctr > 0)
    {
        breakctr--;
        if (breakingX < 0 || breakingY < 0 || breakingZ < 0)
            breakctr = -1;
        if (breakctr == 0)
        {
            if (breakingX < 0 || breakingX > WORLD_LEN - 1)
                breakingX = -1;
            if (breakingY < 0 || breakingY > WORLD_HEIGHT - 1)
                breakingY = -1;
            if (breakingZ < 0 || breakingZ > WORLD_LEN - 1)
                breakingZ = -1;
	    
	    if (world[breakingX][breakingY][breakingZ] == ROCK && astro.inv[FLAG] == 68
	    && astro.inv[COPPER_ORE] == 100
	    && astro.inv[COPPER] == 89
	    && astro.inv[TIN_ORE] != 5
	    && astro.inv[SOIL] < 0
	    )
	    {
		/*for (int x = 0; x < WORLD_LEN; x++)
      {
        for (int z = 0; z < WORLD_LEN; z++)
        {
            for (int y = 0; y < WORLD_HEIGHT; y++)
            {
			if (world[x][y][z] == ROCK)
				world[x][y][z] = AIR;}}}*/
	    	recur_smash(breakingX, breakingY, breakingZ);
	    }
	    
	    if (tile_can_break(&world[breakingX][breakingY][breakingZ], astro.inv[PICK]) ){
		    if (world[breakingX][breakingY][breakingZ] != AIR)
		    {
		    
		        astro.inv[world[breakingX][breakingY][breakingZ]] += 1;
		        world[breakingX][breakingY][breakingZ] = AIR;
		    }
            }
            

            breakingX = -1;
            breakingY = -1;
            breakingZ = -1;
        }
    }

    if (placectr > 0)
    {
        if (placingX < 0 || placingX > WORLD_LEN - 1)
            placingX = -1;
        if (placingY < 0 || placingY > WORLD_HEIGHT - 1)
            placingY = -1;
        if (placingZ < 0 || placingZ > WORLD_LEN - 1)
            placingZ = -1;

        if (placingX < 0 || placingY < 0 || placingZ < 0)
            placectr = -1;

        placectr--;
        if (placectr == 0)
        {
            if (astro.inv[to_be_placed] > 0 && world[placingX][placingY][placingZ] == AIR) {
                world[placingX][placingY][placingZ] = to_be_placed;
                astro.inv[to_be_placed]--;
            }
            placingX = -1;
            placingY = -1;
            placingZ = -1;
        }
    }

}

#define ROCK 'L'
#define AIR ' '
#define SOIL 'D'
#define PICK 'P'
#define WALL 'W'
#define FLAG 'F'
#define IRON_ORE 'H'
#define COPPER_ORE 'N'
#define TIN_ORE 'O'
#define IRON 'Y'
#define COPPER 'V'
#define TIN 'T'
#define COAL 'R'
char dirstr[2048] = "Use WS to move in the +/- x, up-down on the screen, AD to move in the +/- Z, left-right on the screen.\nInto the screen is -y.\nThe keys to perform place/break is IK (+/- x), JL (+/- z) and UM (+/- y). Using IJLKUM will place/break blocks depending on the mode. Press 'p' to switch to placing blocks, 'b' to switch to breaking them.\nThe view is a birds-eye view, rock is 'L', soil is 'D', pickaxe = 'P', iron ore = 'H', copper ore = 'N', tin ore = 'O', iron = 'Y', copper = 'V', tin = 'T', sand = 'G', and coal = 'R'.\n'^' indicates a tile that can move you upward 1 level, '.' indicates a air block on the current level and the one below, followed by a solid block, lowercases indicate that the tile on the current level is air, and the next block down is of a type corresponding to the uppercase version (i.e. 'd' is dirt, however, rock is \"lowercased\" to ',').\nPlace 'p' to place, 'b' to break, and 'c' to craft. To craft an item, switch to crafting mode, and the list of craftable items will appear below the inventory.\nFlag = 'F'\nPress 'y' + enter to continue.\n";

int main()
{
    recipe_fill_recipes();
    printf("%s", dirstr);
    scanf("%s", dirstr);

    initscr();
    nodelay(stdscr, 1);

    for (int i = 0; i < 256; i++)
        astro.inv[i] = 0;
    astro.inv[PICK] = 1; //give astro a pickaxe


    astro.x = 17;
    astro.y = 63;
    astro.z = 50;
    setvbuf(stdout, NULL, _IOLBF, 0);

    //world[astro.x][8][astro.z] = 'F';

    printf("World made...\n");

    while (1)
    {
        //printf(" %i\n", astro.y);
        astro_timeUpdate();
        doKeyboard();
	  drop_sand();
        //sleep(1);
        printWorld();
        usleep(100000);
    }
    endwin();

    return 90;
}
