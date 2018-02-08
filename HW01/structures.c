#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {

	float x;
	float y;
	float z;
} point;

void pointPrintPoint(point p) {
	printf("Point has coordinates (%f, %f, %f) \n", p.x, p.y, p.z);
}

void pointSetZero(point *p) {

	(*p).x = 0.;
	(*p).y = 0.;
	(*p).z = 0.;

	//also valid
        //point pp = *p;
        //pp.x = 0.;
        //pp.y = 0.;
        //pp.z = 0.;

        //also valid
        p->x = 0.;
        p->y = 0.;
        p->z = 0.;


}

float ointDistanceToOrigin(point p) {

	float dist = sqrt(p.x*p.x+p.y*p.y+p.z*p.z);

	return dist;

}

void main() {

	point p;

	//access variables in structure with '.'
	p.x = 1.0;
	p.y = 2.0;
	p.z = 3.0;

	float dist;

	pointSetZero(&p);
	
	pointPrintPoint(p);
}
