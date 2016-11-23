#ifndef POINT3D_H
#define POINT3D_H

typedef struct {
	float x;
	float y;
	float z;
} Point3D;

Point3D point3D_new(float x, float y, float z);

void point3D_print(Point3D* p);

#endif //POINT3D_H