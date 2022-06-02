#include <SDL2/SDL.h>
#ifndef DEFS_H
#define DEFS_H

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280

static SDL_Color white = {255, 255, 255, 255};
static SDL_Color black = {0, 0, 0, 255};
static SDL_Color gray = {20, 20, 20, 255};
static SDL_Color green = {19, 171, 21, 255};
static SDL_Color blue = {0, 135, 252, 255};
static SDL_Color red = {181, 2, 2, 255};
static SDL_Color yellow = {255, 247, 3, 255};

static double G = 6.6743E-11;
static double scale = 1000; // meters per pixel
static double time_scale = 5; // simulated seconds per real second.
static double positions[2][2] = {{SCREEN_WIDTH/2 + 100, SCREEN_HEIGHT/2 + 100},
								 {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 100}};



#endif