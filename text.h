#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define NUM_GLYPHS 128
#define FONT_TEXTURE_SIZE 512
#ifndef TEXT_H
#define TEXT_H

static TTF_Font* font = NULL;
static SDL_Rect glyphs[NUM_GLYPHS];
static SDL_Texture* fontTexture;

void initFont(int fontType, char * filename, SDL_Renderer* renderer);
void drawText(char * text, SDL_Renderer* renderer, int x, int y, int r, int g, int b,int fontType);
SDL_Texture* getTextTexture(char* text, int type);
void drawTextWrapped(char* text, SDL_Renderer* renderer, SDL_Rect box, int r, int g, int b, int fontType);	
int getCharHeight(unsigned char character, int fontType);
int getCharWidth(unsigned char character, int fontType);

#endif
