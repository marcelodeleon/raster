#ifndef FACE_H
#define FACE_H

#include <stdio.h>

typedef struct
{
    int vertexes[3];
    int normals[3];
    int textures[3];

} Face;

Face* face_new(int v1, int n1, int t1, int v2, int n2, int t2, int v3, int n3, int t3);

void face_print(Face* f);

#endif //FACE_H
