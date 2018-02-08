#include <stdio.h>
#include <stdlib.h>

void myPrintf(char*); //function stub

void main() {

	myPrintf("Hello World!");

}

void myPrintf(char *string) {

	while (*string != '\0') {
		char c = *string;
		int i = (int) c;
		printf("%c, this char is acturally an int:%d \n", c, i);
		string  = string + 1;
	}

}

