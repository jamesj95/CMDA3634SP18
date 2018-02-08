#include <stdio.h>

int main()
{
	int a, b, i, common;

	printf("Enter two integers: ");
	scanf("%d %d", &a, &b);

	for(i = 1; i <= a && i <= b; i++)
	{
		if (a % i == 0 && b % i == 0)
		{
			common = i;
		}
	}
	printf("GCD of %d and %d is %d", a, b, common);

	return 0;
}
