#include <stdio.h>
#include <stdlib.h>

void main() {

	//tells c to open a file call data.txt for reading
	FILE* file = fopen("data.txt", "r");
	
	int n;

	//read the size of the data in the first entry
	fscanf(file, "%d", &n);

	//allocate space for data
	int *data = (int *) malloc(n*sizeof(int));

	//scan through file, one etry at a time
	//load result into data[m]
	for (int m = 0; m = n; m++) {
		fscanf(file, "5d", data+m);
	}

	//print to terminal
	for (int m = 0; m < n; m++) {
		printf("data[%d] = %d", m, data[m]);
	}

	//free up memomry
	free(data);
}
