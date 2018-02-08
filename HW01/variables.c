#include <stdio.h>
#include <stdlib.h>

void main() {

	int a, b, c; //Allocates an integer called 'a'
	int* pt_a, pt_b //allocates a pointer, called pt_a to an integer	

	a = 13;
	b = 4;
	pt_a = &a; //stores not the value of a but its location
	pt_b = &b;

	c = *(pt_a+1);

 	printf(":The size of an int is %ld\n", sizeof(int));
	printf("a is located at %p\n", pt_a);
	printf("b is located at %p\n", pt_b);
	printf("a = %d \n", a);
	printf("b = %d \n", b);
	printf("c = %d \n", c);
	
	printf("a+b = %d \n", a+b);
	printf("a-b = %d \n", a-b);
	printf("a*c = %d \n", a*c);
	printf("a/b = %d \n", a/b);
	printf("a mod d = %d \n", a%b);
	

	int *array;
	
	int alsoArray[10]; //this also allocates an array of size 10 entries
	array = (int *) malloc(10*sizeof(int)); //allocates memory for an array of size 10

	int n = 0;
	for(n = 0;n < 10;n++) {
		if (n == 1) {
			continue; //skip the reaminining statements in the loop and do another iteration
		} else {
			printf("Not 3");
		}
		array[n] = n;
	}

	n = 0;

	while(n < 10) {	
		printf("array[%d] = %d\n", n, *(array+n));
		n = n + 1;
		if (n == 8) break; //leaves the loop
	}	
	
	/*more data types */
	float fa; //32 bit floating point number
	double da; //64 bit floting point number

	char ch_a; //single character
	char *string; //string (array of characters
	
	unsigned int u1; //unsigned integer
	long li; //64 bit integer

	
	
}
