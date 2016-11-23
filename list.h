#ifndef LIST_H
#define LIST_H

struct BlockStruct
{
	void* data;
	struct BlockStruct* next;
};

typedef struct BlockStruct Block;

typedef struct List
{
    int dataSize;
    int elementCount;
    Block* head;
    Block* tail;
}List;

//Crea una nueva lista vacía.
List* list_new(int dataSize);

// Devuelve 1 si la lista se encuentra vacía 0 en otro caso.
int list_empty(List* l);

// Devuelve el largo de la lista l.
unsigned int list_size(List* l);

// Agrega el bloque b como ultimo elemento de la lista l.
void list_add(List* l, void* data);

// Inserta el bloque b en la posición indx de la lista l si y solo si 0 <= indx <= n,
// donde n es el largo de la lista l.
void list_insert(List* l, int indx, Block* b); 

// Devuelve la posición en la lista del bloque b o -1 si el bloque o no se encuentra presente.
int list_find(List* l, Block* b);

// Remueve el elemento en la posiciónn indx de la lista l. Devuelve el bloque que se encontraba
// en dicha posición o NULL si indx >= n, donde n es o el largo de la lista.
Block* list_remove(List* l, int indx);

//Elimina la lista, liberando sus recursos, pero no los de sus bloques.
void list_free(List* l);

// Retorna el elemento en la posición index.
void* list_get_nth(List* list, int index);

#endif //LIST_H