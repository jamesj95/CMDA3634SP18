#include <stdio.h>

int main()
{
        int a, b, i, common, leastCommon;

        printf("Enter two integers: ");
        scanf("%d %d", &a, &b);

        for(i = 1; i <= a && i <= b; i++)
        {
                if (a % i == 0 && b % i == 0)
                {
                        common = i;
                }
        }
	leastCommon = (a * b)/ common;

	printf("The least common multiple of two numbers %d and %d is %d.", a, b, leastCommon);

	return 0;
}
