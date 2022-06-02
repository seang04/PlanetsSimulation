#include <SDL2/SDL.h>
#include "target.h"
#include "object.h"
#include "defs.h"

struct target * new_target(double x, double y, double radius){
	struct target * target = (struct target *)malloc(sizeof(struct target));
	target->p = (struct vector *)malloc(sizeof(struct vector));
	target->p->x = x;
	target->p->y = y;
	target->radius = radius;
	return target;
}

int check_target(struct object * object, struct target * target){
	double d = distance(object->p, target->p);	
	//printf("%f, %f\n", d, object->size + target->radius);
	
	if(d < object->size + target->radius){
		return 1;
	}
	else{
		return 0;
	}
}
int check_targets(struct object ** objects, struct target * target, int num_obj){
	for(int i = 0; i < num_obj; i++){
		if(check_target(objects[i], target)){
			double randx = rand() % SCREEN_HEIGHT + SCREEN_HEIGHT/2;
			double randy = rand() % SCREEN_HEIGHT/2 + SCREEN_HEIGHT/2;
			target->p->x = randx;
			target->p->y = randy;
			return 1;
			//printf("%f, %f\n", target->p->x, target->p->y);
		}
	}
	return 0;		

}
