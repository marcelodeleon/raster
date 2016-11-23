#include <stdio.h>
#include "point3D.h"

Point3D point3D_new(float x, float y, float z)
{
	Point3D p;
    p.x = x;
	p.y = y;    
	p.z = z;

	return p;    
}

void point3D_print(Point3D* p)
{
	printf("x:%f\ty:%f\tz:%f\n", p->x, p->y, p->z);
}