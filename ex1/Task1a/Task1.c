#include <stdio.h>
#include <stdlib.h>



void allocate(int value) {
	int *ptr = NULL;
	ptr = malloc(1024*1024*sizeof(char));
	*ptr = value;
	printf("test of allocated memory: %i \n", *ptr);
}

void main(){

	int i = 1;
	while (1){

		allocate(i);
		i++;
	}

}


