#include "obj.h"
#include <stdio.h>
#include <assert.h>
#include "framework/mm.h"

Obj* obj_load(char *filename)
{
	printf("%s\n", "Start obj parser");

	Obj* obj = (Obj *)cg_malloc(sizeof(Obj));
	obj->vertexes = list_new();
	obj->normals = list_new();
	obj->textures = list_new();
	obj->faces = list_new();

	Vertex v;
	v.x = -0.500000;
	v.y = -0.500000;
	v.z = -0.500000;

	Vertex v1;
	v1.x = -0.500000;
	v1.y = -0.500000;
	v1.z = -0.500000;

	Vertex v2;
	v2.x = -0.500000;
	v2.y = -0.500000;
	v2.z = -0.500000;

	printf("Vertex x:%f\ty:%f\tz:%f\n", v.x, v.y, v.z);
    printf("Pointr v %p\n", &v);

	Block* block = (Block *)cg_malloc(sizeof(Block));
	block->data = cg_malloc(sizeof(Vertex));
	block->data = &v;

	Block* b2 = (Block *)cg_malloc(sizeof(Block));
	b2->data = cg_malloc(sizeof(Vertex));
	b2->data = &v1;

	Block* b3 = (Block *)cg_malloc(sizeof(Block));
	b3->data = cg_malloc(sizeof(Vertex));
	b3->data = &v2;

	list_add(obj->vertexes, block);
	list_add(obj->vertexes, b2);
	list_add(obj->vertexes, b3);

	assert(list_size(obj->vertexes) == 3);

	printf("Pointr bd %p\n", block->data);
	printf("Size v %d\n", sizeof(Vertex));

	Vertex * p = (Vertex *)block->data;

	printf("Vertex from data x:%f\ty:%f\tz:%f\n", block->data->x, p->y, p->z);
    return obj;
}

void obj_render(Obj * obj)
{
    printf("%s\n", "obj render");
}

void obj_free (Obj * obj)
{
	printf("%s\n", "obj free");
}