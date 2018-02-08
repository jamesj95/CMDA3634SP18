#include <stdio.h>

int main()
{
        int a, i, prime = 0;

	printf("Enter an Integer: ");
	scanf("%d", &a);

	for (i = 1; i <= a; i++)
	{
		if (a % i == 0)
		{
			prime++;
		}
	}
	if (prime == 2) {
		printf("%d is prime.", a);
	}
	else {
		printf("%d is not prime.", a);
	}

        return 0;
}

