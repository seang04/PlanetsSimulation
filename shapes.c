#include <SDL2/SDL.h>
#include <math.h>
#include "vector.h"
#include "defs.h"
#include "shapes.h"


void circle(SDL_Renderer * renderer, SDL_Color color, double xc, double yc,
			double inner, double outer){
	int xo = outer;
    int xi = inner;
    int y = 0;
    int erro = 1 - xo;
    int erri = 1 - xi;

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    while(xo >= y) {
    	SDL_RenderDrawLine(renderer, xc + xi,  yc + y, xc + xo, yc + y);
    	SDL_RenderDrawLine(renderer, xc + y,  yc + xi, xc + y, yc + xo);
   		SDL_RenderDrawLine(renderer, xc - xo, yc + y, xc - xi, yc + y);
    	SDL_RenderDrawLine(renderer, xc - y,  yc + xi, xc - y, yc + xo);
    	SDL_RenderDrawLine(renderer, xc - xo, yc - y, xc - xi, yc - y);
    	SDL_RenderDrawLine(renderer, xc - y,  yc - xo, xc - y, yc - xi);
    	SDL_RenderDrawLine(renderer, xc + xi, yc - y, xc + xo, yc - y);
    	SDL_RenderDrawLine(renderer, xc + y,  yc - xo, xc + y, yc - xi);
    	y++;

    	if (erro < 0) {
    		erro += 2 * y + 1;
    	} 
    	else {
    		xo--;
    		erro += 2 * (y - xo + 1);
    	}
    	
    	if (y > inner) {
    		xi = y;
    	} 
    	else {
    		if (erri < 0) {
    			erri += 2 * y + 1;
    		} 
    		else {
    			xi--;
    		    erri += 2 * (y - xi + 1);
            }
        }   		                   	                                        	                    	          
    }   			
}
void drawCircle(SDL_Renderer * renderer, SDL_Color color, double centerX, double centerY, 
			    double radius){
	circle(renderer, color, centerX, centerY, radius - 1, radius);
}
void drawThickCircle(SDL_Renderer* renderer, SDL_Color color, double centerX, double centerY, 
				     double radius, double thickness){
	circle(renderer, color, centerX, centerY, radius - thickness, radius);
}

void drawArrow(SDL_Renderer* renderer, SDL_Color color, 
			   struct vector* start, double length, double angle){
	double radians = angle;// * (180.0/M_PI);
	double x2 = start->x + cos(radians) * length;
	double y2 = start->y + sin(radians) * length;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, start->x, SCREEN_HEIGHT - start->y, x2, SCREEN_HEIGHT - y2);

	SDL_RenderDrawLine(renderer, x2, SCREEN_HEIGHT - y2, 
					   start->x + cos(radians - M_PI/6) * (length - 10), 
					   SCREEN_HEIGHT - (start->y + sin(radians - M_PI/6) * (length - 10)));
	SDL_RenderDrawLine(renderer, x2, SCREEN_HEIGHT - y2, 
					   start->x + cos(radians + M_PI/6) * (length - 10), 
					   SCREEN_HEIGHT - (start->y + sin(radians + M_PI/6) * (length - 10)));
	
	
	
}