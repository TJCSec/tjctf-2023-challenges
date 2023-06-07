#include <stdio.h>
#include <stdlib.h>

#define N 256

int i;

int main()
{
	setbuf(stdout, NULL);

	char buffer[N];

	// leak stack address
	printf("0x%lx\n", (unsigned long)buffer);

	// overwrite return address
	fgets(buffer, 2 * N, stdin);

	// disallow syscall instruction
	for (i = 0; i < 2 * N - 1; i++)
	{
		if (buffer[i] == '\0')
		{
			break;
		}

		if (buffer[i] == '\x0f' && buffer[i + 1] == '\x05')
		{
			puts("nonono");
			exit(1);
		}
	}

	puts("ok");
}
