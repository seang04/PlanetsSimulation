#include <SDL2/SDL.h>
#include "vector.h"

#ifndef SHAPE_H
#define SHAPE_H

void cirlce(SDL_Renderer * renderer, SDL_Color color, 
			double centerX, double centerY, double inner, double outer); 
void drawCircle(SDL_Renderer * renderer, SDL_Color color, 
		        double centerX, double centerY,  double radius);
void drawThickCircle(SDL_Renderer * renderer, SDL_Color color,
				     double centerX, double centerY, double radius, double thickness);
void drawArrow(SDL_Renderer * renderer, SDL_Color color, 
			   struct vector * start, double length, double angle);

#endif