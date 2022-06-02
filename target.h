#include "object.h"
#include "vector.h"
#include <SDL2/SDL.h>

#ifndef TARGET_H
#define OBJECT_H

struct target {
	struct vector* p;
	double radius;
} target;

struct target* new_target(double x, double y, double radius);
int check_target(struct object * object, struct target * target);
int check_targets(struct object ** objects, struct target * target, int num_obj);

#endif