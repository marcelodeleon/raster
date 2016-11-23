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

	printf("Point3D x:%f\ty:%f\tz:%f\n", v.x, v.y, v.z);
    printf("Pointr v %p\n", &v);

	list_add(obj->vertexes, &v);
	list_add(obj->vertexes, &v1);
	list_add(obj->vertexes, &v2);
	assert(list_size(obj->vertexes) == 3);

	//NORMALS
	Point3D vn = point3D_new(1.000000, 0.000000, 0.000000);
	list_add(obj->normals, &vn);
	assert(list_size(obj->normals) == 1);

	//TEXTURES
	Point2D t1 = point2D_new(0.000000, 3.000000);
	Point2D t2 = point2D_new(3.000000, 0.000000);
	Point2D t3 = point2D_new(0.000000, 0.000000);

	list_add(obj->textures, &t1);
	list_add(obj->textures, &t2);
	list_add(obj->textures, &t3);
	assert(list_size(obj->textures) == 3);

	Point3D * p = (Point3D *)(((Block *)obj->vertexes->head)->data);
 
	printf("Point3D from data x:%f\ty:%f\tz:%f\n", p->x, p->y, p->z);
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