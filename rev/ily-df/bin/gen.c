#include "stdio.h"
#include "stdlib.h"
#include "perlin.h"
#include "flag.h"
#define WORLD_LEN 128
#define WORLD_HEIGHT 64

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
#define SAND 'G'

char rand_ore()
{
	char chr = COAL;

	int r = rand() % 1000;

	if (r > 200)
	{
		chr = COPPER_ORE;
	}
	if (r > 500)
	{
		chr = TIN_ORE;
	}
	if (r > 800)
	{
		chr = IRON_ORE;
	}

	return chr;
}

char world[WORLD_LEN][WORLD_HEIGHT][WORLD_LEN];
float height[WORLD_LEN][WORLD_LEN];

void makeWorld(int seed)
{
	for (int x = 0; x < WORLD_LEN; x++)
	{
		for (int z = 0; z < WORLD_LEN; z++)
		{
			float noiseH = fnoise(x, z) + 15;
			height[x][z] = noiseH;
			for (int y = 0; y < WORLD_HEIGHT; y++)
			{
				if (y > noiseH)
				{
					world[x][y][z] = AIR;
				}
				else
				{
					if (y > noiseH - 2)
						world[x][y][z] = SOIL;
					else
					{
					top:
						int chose = rand() % 1000;
						world[x][y][z] = AIR;
						if (chose < 900)
							world[x][y][z] = ROCK;
						if (chose >= 900)
							world[x][y][z] = SAND;
						if (chose >= 950)
							world[x][y][z] = COAL;
						if (chose > 960)
							world[x][y][z] = TIN_ORE;
						if (chose > 975)
							world[x][y][z] = COPPER_ORE;
						if (chose > 990)
							world[x][y][z] = IRON_ORE;

						if (y == 1 && world[x][y][z] == SAND)
							goto top;
						if (y == 0)
						{
							world[x][y][z] = ROCK;
						}
					}
				}
			}
		}
	}
	for (int zMin = 0; zMin < 100; zMin += 45)
	{
		for (int x = 0; x < 120; x++)
		{
			for (int z = 0; z < 26; z++)
			{
				// printf("height at %i, %i: %f\n",x,z,height[x][z]);
				if (flag[120 - x][z])
				{
					int oh = 0;
					for (int y = 0; y < height[x][z + zMin]; y++)
					{
						if (world[x][y][z + zMin] != ROCK)
						{
							oh = y;
							break;
						}
					}
					world[x][oh][z + zMin] = SAND;
				}
				else
				{

					// world[x][3][z] = rand_ore();
					for (int y = 1; y < height[x][z + zMin] + 5; y++)
					{
						if (world[x][y][z + zMin] == SAND)
						{
							world[x][y][z + zMin] = rand_ore();
							world[x][y + 1][z + zMin] = SAND;
							break;
						}
					}
				}
			}
		}
	}
}

int main()
{
	makeWorld(100);

	printf("#define WORLD_LEN 128\n#define WORLD_HEIGHT 64\nchar world[WORLD_LEN][WORLD_HEIGHT][WORLD_LEN] = {");
	for (int x = 0; x < WORLD_LEN; x++)
	{
		printf("{");
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			printf("{");
			for (int z = 0; z < WORLD_LEN; z++)
			{
				printf("'%c'", world[x][y][z]);
				if (z != WORLD_LEN - 1)
					printf(",");
			}
			printf("}");
			if (y != WORLD_HEIGHT - 1)
				printf(",");
		}
		printf("}");
		if (x != WORLD_LEN - 1)
			printf(",");
	}
	printf("};");
	return 0;
}
