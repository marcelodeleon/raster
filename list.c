#include <stdio.h>
#include <assert.h>
#include "list.h"
#include "framework/mm.h"

List* list_new(int dataSize)
{

	List* list = (List *)cg_malloc(sizeof(List));
    list->dataSize = dataSize;
    list->elementCount = 0;
	list->head = NULL;
	list->tail = NULL;

	return list;
}

void list_add(List* l, void* data)
{	
	Block* b = (Block *)cg_malloc(sizeof(Block));
	b->data = cg_malloc(l->dataSize);
	b->data = data;

	//Si la lista no contiene elementos tail y head son el mismo bloque
	if(l->elementCount == 0)
	{
        l->head = b;
        l->tail = b;
    } else 
    {
        l->tail->next = b;
        l->tail = b;
        l->tail->next = NULL;
    }

	l->elementCount++;
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

void* list_get_nth(List* list, int index)
{
	Block* current = list->head;
	int count = 0;

	// Asegura que el índice sea positivo
	if(index < 0)
	{
		printf("%s\n", "Indice negativo");
		assert(0);
	}

	// Chequea que la lista no esté vacía
	if(list_empty(list) == 0)
	{
		printf("%s\n", "Lista Vacía!");
		assert(0);
	}

	while(count != index)
	{
		current = current->next;
		count++;
	}

	return current->data;
}

void list_free(List* l)
{
	// Si tail y head apuntan al mismo espacio de memoria solamente liberamos uno.
	if (l->head == l->tail)
	{
		cg_free(l->head);	
	}else {
		cg_free(l->head);
	    cg_free(l->tail);	
	}
	
	cg_free(l);
}
