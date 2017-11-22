#ifndef OBJ_H
#define OBJ_H

#include "list.h"
#include "point3D.h"
#include "point2D.h"
#include "face.h"

typedef struct
{
    char* name;
    List* vertexes;
    List* normals;
    List* textures;
    List* faces;
} Obj;

Obj* obj_new();

Obj* obj_load(char *filename);

void obj_render(Obj * obj);

void obj_free (Obj * obj);

#endif //OBJ_H
