#include <stdio.h>
#include "framework/mm.h"
#include "point3D.h"

Point3D* point3D_new(float x, float y, float z)
{
	Point3D *p = (Point3D *) cg_malloc(sizeof(Point3D));
    p->x = x;
	p->y = y;
	p->z = z;

	return p;    
}

void point3D_print(Point3D* p)
{
	printf("x:%f\ty:%f\tz:%f\n", p->x, p->y, p->z);
}
