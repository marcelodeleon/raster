#include "point3D.h"

Point3D point3D_new(float x, float y, float z)
{
	Point3D p;
    p.x = x;
	p.y = y;    
	p.z = z;

	return p;    
}