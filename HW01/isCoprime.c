#include <stdio.h>

int main()
{
        int a, b, i, common, coPrime;

        printf("Enter two integers: ");
        scanf("%d %d", &a, &b);

        for(i = 1; i <= a && i <= b; i++)
        {
                if (a % i == 0 && b % i == 0)
                {
                        common = i;
                }
        }

	if(common == 1)
	{	
        	printf("Co prime of %d and %d is %d.", a, b, common);
	}
	else 
	{
		printf("Co prime of %d and %d does not exist.", a, b);
	}

        return 0;

}

