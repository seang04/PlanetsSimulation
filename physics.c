#include <math.h>

#include "defs.h"
#include "object.h"

struct vector* calculate_gravity(struct object* a, struct object * b){
	double d_x = (a->p->x - b->p->x) * (scale);
	double d_y = (a->p->y - b->p->y) * (scale);
	//printf("%f, %f\n", d_x, d_y);
	double r = sqrt(pow(d_x, 2) + pow(d_y, 2));
	double magnitude = (G * a->mass * b->mass) / pow(r, 2);
	double angle = atan(d_y/d_x);
	if(d_x < 0) angle = angle + M_PI;
	struct vector * force = (struct vector *)malloc(sizeof(struct vector));
	force->x = magnitude * cos(angle);
	force->y = magnitude * sin(angle);
	return force;
}