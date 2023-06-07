#include <stdio.h>
#include <stdlib.h>

int main()
{
	setbuf(stdout, NULL);

	char str[153] = "Nothing to see here... Nothing to see here...";

	FILE* f = fopen("flag.txt", "r");
	
	if (f == NULL) {
		printf("Cannot find flag.txt.");
		return 1;
	}
	
	fgets(&str[128], 25, f);

	fclose(f);

	printf("Input: ");
	scanf("%15s",str);

	int val = atoi(str);

	if (val > 128) //no big values
		return 0;
	
	unsigned char c = (unsigned char) val; //no small values
	
	printf("%s", &str[c]);
	return 0;
}
