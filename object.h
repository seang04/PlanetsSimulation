#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include "vector.h"

#ifndef OBJECT_H
#define OBJECT_H


struct object {
	double mass; // kg
	double size; // for drawing and collison handling
	struct vector* v; // m/s 
	struct vector* p; // position
	struct vector* n_force; // N
	SDL_Texture * txtr;
};

struct object* new_object(double mass, double size, double v_x, double v_y, double p_x, double p_y);
void free_object(struct object* object);
void remove_object(struct object ** objects, int index, int * num_obj);
void update_objects(struct object ** objects, int * num);
void draw_objects(SDL_Renderer * renderer, struct object ** objects, int num);
SDL_Texture * generate_gravity_field(SDL_Renderer * renderer, struct object * objects);
int obj_collision(struct object * obj1, struct object * obj2);


#endif
