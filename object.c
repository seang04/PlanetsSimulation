#include <stdlib.h>
#include <SDL2/SDL.h>

#include "object.h"
#include "physics.h"
#include "defs.h"
#include "shapes.h"

struct object* new_object(double mass, double size, double v_x, double v_y, double p_x, double p_y){
	struct object* object = (struct object*)malloc(sizeof(struct object));
	object->v = (struct vector*)malloc(sizeof(struct vector));
	object->p = (struct vector*)malloc(sizeof(struct vector));
	object->n_force = (struct vector*)malloc(sizeof(struct vector));
	object->v->x = v_x;
	object->v->y = v_y;
	object->p->x = p_x;
	object->p->y = p_y;
	object->n_force->x = 0;
	object->n_force->y = 0;
	object->mass = mass;
	object->size = size;
	object->txtr = NULL;
	
	return object; 
}
void free_object(struct object* object){
	free(object->v);
	free(object->p);
	free(object->n_force);
	free(object);
}
void remove_object(struct object ** objects, int index, int * num_obj){
	free_object(objects[index]);
	for(int i = index; i < *num_obj - 1; i++){
		objects[i] = objects[i+1];
	}
	*num_obj = *num_obj - 1;
}

void update_objects(struct object ** objects, int * num){
	for(int i = 0; i < *num; i++){
		objects[i]->p->x += objects[i]->v->x * 0.01f * (1/scale) * time_scale;
		objects[i]->p->y += objects[i]->v->y * 0.01f * (1/scale) * time_scale;
		struct vector* n_force = (struct vector*)malloc(sizeof(struct vector));
		n_force->x = n_force->y = 0;
		struct vector* tmp_force = NULL;
		int break_flag = 0;
		for(int j = 0; j < *num; j++){
			if(j != i){
				if(obj_collision(objects[i], objects[j])){
					if(objects[i]->mass == objects[j]->mass){
						remove_object(objects, i, num);
						remove_object(objects, j, num);
						break_flag = 1;
						break;
					}
					else if(objects[i]->mass < objects[j]->mass){
						remove_object(objects, i, num);
						break_flag = 1;
						break;
					}
					else if(objects[i]->mass > objects[j]->mass){
						remove_object(objects, j, num);
						continue;
					}
				}
				tmp_force = calculate_gravity(objects[j], objects[i]);
				add_vector(n_force, tmp_force);
			}
		}
		if(break_flag) break;
		if(tmp_force != NULL){ 
			free(tmp_force);
		}
		objects[i]->n_force->x =  n_force->x;
		objects[i]->n_force->y =  n_force->y;
		struct vector* acceleration = (struct vector*)malloc(sizeof(struct vector));
		acceleration->x = n_force->x / objects[i]->mass;
		acceleration->y = n_force->y / objects[i]->mass;
		objects[i]->v->x += acceleration->x * 0.01f * time_scale;
		objects[i]->v->y += acceleration->y * 0.01f * time_scale;
		free(n_force);
		free(acceleration);
		if(objects[i]->p->x > SCREEN_WIDTH + 100 || objects[i]->p->x < -100){
			remove_object(objects, i, num);
			continue;
		}
		if(objects[i]->p->y > SCREEN_HEIGHT + 100 || objects[i]->p->y < -100){
			remove_object(objects, i, num);
			continue;
		}
	}
}
void draw_objects(SDL_Renderer * renderer, struct object ** objects, int num){
	for(int i = 0; i < num; i++){
		double x = objects[i]->p->x; //* (1.0/scale);
		double y = SCREEN_HEIGHT - objects[i]->p->y; //* (1.0/scale));
		double size = objects[i]->size;

		if(objects[i]->txtr == NULL){
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			drawThickCircle(renderer, white, x, y, objects[i]->size, 5);
			SDL_RenderDrawPoint(renderer, x, y);
		}
		else{
			SDL_Rect dest = {x - size, y - size, size * 2, size * 2};
			SDL_RenderCopy(renderer, objects[i]->txtr, NULL, &dest);
		}
		
		/*double force_angle = atan(objects[i]->n_force->y/objects[i]->n_force->x);
		double force_mag = log2(magnitude(objects[i]->n_force)) + 10;
		double velocity_angle = atan(objects[i]->v->y/objects[i]->n_force->x);
		double velocity_mag = log2(magnitude(objects[i]->v)) + 10;
		if(objects[i]->n_force->x < 0) force_angle += M_PI;
		//struct vector v_p = {objects[i]->p + objects, 0};
		//drawArrow(renderer, white, objects[i]->p, force_mag, force_angle);
	    //drawArrow(renderer, white, objects[i]->p, velocity_mag, velocity_angle);*/
	}
}

SDL_Texture * generate_gravity_field(SDL_Renderer* renderer, struct object* objects){
	SDL_Surface * surface;
	surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0xff);
	SDL_Renderer * temp_renderer = SDL_CreateSoftwareRenderer(surface);
	
	//double x = objects->p->x;
	//double y = SCREEN_HEIGHT - objects->p->y;
	int r = objects->size - 1;
	double str;
	double str_max = str = (G * objects->mass)/pow(r, 2);
	SDL_Color color = {255, 0, 0, 255}; 
	SDL_SetRenderDrawBlendMode(temp_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(temp_renderer,0, 0, 0, 0);
	SDL_RenderClear(temp_renderer);
	drawThickCircle(temp_renderer, color, objects->p->x, SCREEN_HEIGHT - objects->p->y, 
					objects->size, objects->size);
	for(int i = 1; i <= SCREEN_WIDTH; i++){
		for(int j = 1; j <= SCREEN_HEIGHT; j++){
			struct vector pxl = {i, j};
			SDL_SetRenderDrawColor(temp_renderer, color.r, color.b, color.g, color.a);
			//r++;
			double d = distance(&pxl, objects->p);
			str = (G * objects->mass)/pow(d, 2);
			color.r = 255 * (str/str_max);
			//printf("%d, %d, %d, %f, %f\n", i, j, color.a, str, str/str_max);
			SDL_RenderDrawPoint(temp_renderer, i, j);
			//printf("%f, %d, %d\n", str/str_max, r, color.a);
		}
		
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_Texture * texture;
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); 
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_DestroyRenderer(temp_renderer);
	return texture;
}

int obj_collision(struct object * obj1, struct object * obj2){
	double d = distance(obj1->p, obj2->p);
	if(d < obj1->size + obj2->size){
		return 1;
	}
	else{
		return 0;
	}
	
}
