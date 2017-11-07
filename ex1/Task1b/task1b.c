#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"


int main(){
	printf("main.. \n");
	
	//Variables
	//list_t list;
	
	//Populate list
	list_t list = list_create();
	list_print(list);
	
	
	return 0;
}
