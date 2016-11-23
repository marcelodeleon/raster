#ifndef POINT2D_H
#define POINT2D_H

typedef struct {
	float x;
	float y;
} Point2D;

Point2D point2D_new(float x, float y);

#endif //POINT2D_H