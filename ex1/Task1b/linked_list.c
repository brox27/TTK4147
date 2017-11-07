#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

void test()
{
	printf("Testing \n");
}

list_t list_create()
{	
	list_t list = (list_t) malloc(sizeof(struct list));	
	list->head = NULL;
	list->tail = NULL;
	list->length = 0;
	
	return list;
}

void list_delete(list_t list)
{ 
	struct node* node = list->head;
	while(node != NULL){
		struct node* temp = node;
		node = node->next;
		free(temp);
	}
	free(list);
}

void list_insert(list_t list, int index, int data)
{
	struct node* node = (struct node*) malloc(sizeof(struct node));
	node->data = data;
	node->next = NULL;
	node->prev = NULL;

	if(index == 0){
		list->head = node;
		list->tail = node;		
	}else if(index == list->length) {
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	}else{
		int i;
		struct node* tempNode = list->head;
		for(i = 0; i < index - 1; i++){
			tempNode = tempNode->next;
		}
		node->next = tempNode->next;
		node->prev = tempNode;
		tempNode->next->prev = node;
		tempNode->next = node;

	}
	list->length++;
}

void list_append(list_t list, int data)
{
	list_insert(list, list->length, data);
}

void list_print(list_t list)
{
	int i;
	struct node* node = list->head;
	for(i = 0; i < list->length; i++){
		printf("%i ", node->data);
		node = node->next;
	}
	printf("\n");
}

long list_sum(list_t list)
{
	int i;
	int count = 0;
	struct node* node = list->head;
	for(i = 0; i < list->length; i++){
		count+= node->data;
		node = node->next;
	}
	return count;
}

int list_get(list_t list, int index)
{
	int i;
	struct node* node = list->head;
	for(i = 0; i < index; i++){
		node = node->next;
	}
	return node->data;
}

int list_extract(list_t list, int index)
{	
	int i;
	int data;
	struct node* node = list->head;	
//	printf("index %i, length %i \n", index, list->length);
	
	if(list->length == 0){
		return -1;
	}
	else if(index == 0){		
	//	printf("Removing first node \n");

		if(list->length > 1){
			list->head = node->next;
			node->next->prev = NULL;
		}else{
			list->head = NULL;
		}
		
	}else{
	
		for(i = 0; i < index; i++){
			node = node->next;
		}
	
		if(index == list->length) {			
			list->tail = node->prev;
			list->tail->next = NULL;
		}else{							
			node->next->prev = node->prev;
			node->prev->next = node->next;
		}
	}
	data = node->data;
	free(node);
	list->length--;
	return data;
}
