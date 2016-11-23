#include "obj.h"
#include <stdio.h>
#include <assert.h>
#include "framework/mm.h"

Obj* obj_load(char *filename)
{
	printf("%s\n", "Start obj parser");

	//Initialize Obj struct
	Obj* obj = (Obj *)cg_malloc(sizeof(Obj));
	obj->vertexes = list_new(sizeof(Point3D));
	obj->normals = list_new(sizeof(Point3D));
	obj->textures = list_new(sizeof(Point2D));
	obj->faces = list_new(sizeof(Face));

	//VERTEXES
	Point3D v = point3D_new(-0.500000, -0.500000, -0.500000);
	Point3D v1 = point3D_new(0.500000, 0.500000, -0.500000);
	Point3D v2 = point3D_new(-0.500000, 0.500000, -0.500000);

    printf("Pointr v %p\n", &v);

	list_add(obj->vertexes, &v);
	list_add(obj->vertexes, &v1);
	list_add(obj->vertexes, &v2);
	assert(list_size(obj->vertexes) == 3);

	printf("%s\n\n\n", "");
	printf("%s\n", "VERTEXES");
	printf("%s\n", "v1");
	point3D_print((Point3D *)list_get_nth(obj->vertexes, 0));
	printf("%s\n", "v2");
	point3D_print((Point3D *)list_get_nth(obj->vertexes, 1));
	printf("%s\n", "v3");
	point3D_print((Point3D *)list_get_nth(obj->vertexes, 2));
	printf("%s\n\n\n", "");

	//NORMALS
	Point3D vn = point3D_new(1.000000, 0.000000, 0.000000);
	list_add(obj->normals, &vn);
	assert(list_size(obj->normals) == 1);

	
	printf("%s\n", "NORMALS");
	printf("%s\n", "n1");
	point3D_print((Point3D *)list_get_nth(obj->normals, 0));
	printf("%s\n\n\n", "");

	//TEXTURES
	Point2D t1 = point2D_new(0.000000, 3.000000);
	Point2D t2 = point2D_new(3.000000, 0.000000);
	Point2D t3 = point2D_new(0.000000, 0.000000);

	list_add(obj->textures, &t1);
	list_add(obj->textures, &t2);
	list_add(obj->textures, &t3);
	assert(list_size(obj->textures) == 3);

	printf("%s\n", "TEXTURES");
	printf("%s\n", "t1");
	point3D_print((Point3D *)list_get_nth(obj->textures, 0));
	printf("%s\n", "t2");
	point3D_print((Point3D *)list_get_nth(obj->textures, 1));
	printf("%s\n", "t3");
	point3D_print((Point3D *)list_get_nth(obj->textures, 2));
	printf("%s\n\n\n", "");

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