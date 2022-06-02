#ifndef VECTOR_H
#define VECTOR_H

struct vector {
	double x;
	double y;
} vector;

//all vector math functions modify vector a 
void add_vector(struct vector* a, struct vector* b);
double magnitude(struct vector* vector);
void dot_product(struct vector* a, struct vector* b);
void cross_product(struct vector* a, struct vector* b);
double distance(struct vector* a, struct vector * b);

#endif