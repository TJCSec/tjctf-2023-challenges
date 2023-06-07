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

#define maxIngredients 15

struct CraftingRecipe
{
	char* namestr;
	char ingredients[maxIngredients];
	int ingredientCount[maxIngredients];
	int numIngredients;
	char out;
};


struct CraftingRecipe recipes[100];
int numRecipes = 0;

int recipe_can_craft(struct CraftingRecipe* recipe, int* inv)
{
	for (int i = 0; i < recipe->numIngredients; i++)
	{
		if (recipe->ingredientCount[i] > inv[recipe->ingredients[i]])
			return 0;
		
	}
	return 1;
}

void recipe_craft(struct CraftingRecipe* recipe, int* inv)
{
	if (! recipe_can_craft(recipe,inv))
		return;

	for (int i = 0; i < recipe->numIngredients; i++)
		inv[recipe->ingredients[i]] -= recipe->ingredientCount[i];
	inv[recipe->out]++;
}

void recipe_push_recipe(struct CraftingRecipe* recipe, struct CraftingRecipe* list, int* place)
{
	if (*place >= 100)
		return;
	list[*place] = *recipe;
	(*place)++;
}

void recipe_fill_recipes()
{
    struct CraftingRecipe simple;
    simple.namestr = "Wall";
    simple.ingredients[0] = ROCK;
    simple.ingredientCount[0] = 1;
    simple.numIngredients = 1;
    simple.out = WALL;
    
    recipe_push_recipe(&simple, recipes, &numRecipes);
    
    simple.namestr = "Flag";
    simple.ingredients[0] = ROCK;
    simple.ingredients[1] = SOIL;
    simple.ingredients[2] = 'u';
    simple.ingredientCount[0] = 100000;
    simple.ingredientCount[1] = 10000;
    simple.ingredientCount[2] = 10;
    simple.numIngredients = 3;
    simple.out = FLAG;
    
    recipe_push_recipe(&simple, recipes, &numRecipes);
    
    simple.namestr = "Tin";
    simple.ingredients[0] = TIN_ORE;
    simple.ingredients[1] = COAL;
    simple.ingredientCount[0] = 1;
    simple.ingredientCount[1] = 1;
    simple.numIngredients = 2;
    simple.out = TIN;
    
    recipe_push_recipe(&simple, recipes, &numRecipes);
    
    simple.namestr = "Iron";
    simple.ingredients[0] = IRON_ORE;
    simple.ingredients[1] = COAL;
    simple.ingredientCount[0] = 1;
    simple.ingredientCount[1] = 1;
    simple.numIngredients = 2;
    simple.out = IRON;
    
    recipe_push_recipe(&simple, recipes, &numRecipes);
    
    simple.namestr = "Copper";
    simple.ingredients[0] = COPPER_ORE;
    simple.ingredients[1] = COAL;
    simple.ingredientCount[0] = 1;
    simple.ingredientCount[1] = 1;
    simple.numIngredients = 2;
    simple.out = COPPER;
    
    recipe_push_recipe(&simple, recipes, &numRecipes);
}

