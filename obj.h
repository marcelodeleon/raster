#ifndef OBJ_H
#define OBJ_H

#include "list.h"
#include "point3D.h"

typedef struct
{
	float x;
	float y;
} Point2D;

typedef struct
{
    int vertexes[3];
    int normals[3];
    int textures[3];
} Face;

typedef struct
{
    //char* name;
    List* vertexes;
    List* normals;
    List* textures;
    List* faces;
} Obj;

Obj* obj_load(char *filename);

void obj_render(Obj * obj);

void obj_free (Obj * obj);

#endif //OBJ_H