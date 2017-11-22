#include <stdio.h>
#include "framework/mm.h"
#include "face.h"

Face* face_new(int v1, int n1, int t1, int v2, int n2, int t2, int v3, int n3, int t3)
{
	Face *f = (Face *) cg_malloc(sizeof(Face));
	f->vertexes[0] = v1-1;
	f->vertexes[1] = v2-1;
	f->vertexes[2] = v3-1;

	f->normals[0] = n1-1;
	f->normals[1] = n2-1;
	f->normals[2] = n3-1;

	f->textures[0] = t1-1;
	f->textures[1] = t2-1;
	f->textures[2] = t3-1;

	return f;
}

void face_print(Face* f)
{
	printf("Vertexes indexes: %d/%d/%d\n", f->vertexes[0], f->vertexes[1], f->vertexes[2]);
    printf("Normals indexes: %d/%d/%d\n", f->normals[0], f->normals[1], f->normals[2]);
    printf("Textures indexes: %d/%d/%d\n", f->textures[0], f->textures[1], f->textures[2]);
}

