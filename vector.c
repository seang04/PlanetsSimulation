#include <math.h>
#include "vector.h"


void add_vector(struct vector* a, struct vector* b){
	a->x = a->x + b->x;
	a->y = a->y + b->y;
}
double magnitude(struct vector* vector){
	return sqrt(pow(vector->x, 2) + pow(vector->y, 2));
}
double distance(struct vector* a, struct vector* b){
	double x = a->x - b->x;
	double y = a->y - b->y;
	return sqrt(pow(x,2) + pow(y,2));
}