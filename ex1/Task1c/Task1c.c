#include <stdio.h>
#include <stdlib.h>

// Malloc could return a NULL ptr. on failure
// 


void allocate(int value) {
	int *ptr = NULL;
	ptr = malloc(1024*1024*sizeof(char));
	if (ptr == NULL){
		perror("INVALID BLOCK OF MEMORY!! : ");
		exit(-1);
	}
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


