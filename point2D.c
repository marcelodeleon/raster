#include <stdio.h>
#include "point2D.h"

Point2D point2D_new(float x, float y)
{
	Point2D p;
    p.x = x;
	p.y = y;    

	return p;    
}

void point2D_print(Point2D* p)
{
	printf("x:%f\ty:%f\n", p->x, p->y);
}