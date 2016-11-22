#include <stdio.h>
#include "list.h"
#include "framework/mm.h"

List* list_new()
{

	List* list = (List *)cg_malloc(sizeof(List));
    list->elementCount = 0;
	list->head = NULL;
	list->tail = NULL;

	return list;
}

void list_free(List* l)
{
	cg_free(l->head);
	cg_free(l->tail);
	cg_free(l);
}

void list_add(List* l, Block* b)
{	
	//Si la lista no contiene elementos tail y head son el mismo bloque
	printf("Element count %d\n", l->elementCount);
	if(l->elementCount == 0)
	{
		printf("%s\n", "No elements");
        l->head = b;
        printf("%s\n", "added head");
        l->tail = b;
        printf("%s\n", "added tail");
    } else 
    {
    	printf("%s\n", "Elements!");
        l->tail->next = b;
        printf("%s\n", "Next");
        l->tail = b;
        printf("%s\n", "tail!");
    }

	l->elementCount++;
	printf("%s\n", "yeeeey");
}

unsigned int list_size(List* l)
{
	return l->elementCount;
}

int list_empty(List* l)
{
	if(l->elementCount > 0)
	{
		return 1;
	}

	return 0;
}