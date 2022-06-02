#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text.h"

void initFont(int fontType, char* filename, SDL_Renderer* renderer){
	SDL_Surface * surface, *text;
	SDL_Rect dest;
	int i;
	char c[2];
	SDL_Rect *g;
	SDL_Color white = {255, 255, 255, 255};
	memset(&glyphs, 0, sizeof(SDL_Rect) * NUM_GLYPHS);

	font = TTF_OpenFont(filename, 24);

	surface = SDL_CreateRGBSurface(0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff);

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

	dest.x = dest.y = 0;

	for (i = ' ' ; i <= 'z' ; i++){
		c[0] = i;
		c[1] = 0;
		
		text = TTF_RenderText_Blended(font, c, white);

		TTF_SizeText(font, c, &dest.w, &dest.h);

		if (dest.x + dest.w >= FONT_TEXTURE_SIZE){
			dest.x = 0; 
			dest.y += dest.h + 1;
			if (dest.y + dest.h >= FONT_TEXTURE_SIZE){
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, 
				"Out of glyph space in %dx%d font atlas texture map.", 
				FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE);
				exit(1);
			}
		}
		SDL_BlitSurface(text, NULL, surface, &dest);
		g = &glyphs[i];
		g->x = dest.x;
		g->y = dest.y;
		g->w = dest.w;
		g->h = dest.h;

		SDL_FreeSurface(text);

		dest.x += dest.w;
	}

	fontTexture = SDL_CreateTextureFromSurface(renderer, surface);
	
}

void drawText(char* text, SDL_Renderer* renderer, int x, int y, int r, int g, int b, int fontType){

	int i, character;
	SDL_Rect *glyph, dest;

	SDL_SetTextureColorMod(fontTexture, r, g, b);
	
	i = 0;
	character = text[i++];
	while(character){
		glyph = &glyphs[character];

		dest.x = x; 
		dest.y = y;
		dest.w = glyph->w;
		dest.h = glyph->h;

		SDL_RenderCopy(renderer, fontTexture, glyph, &dest);

		x += glyph->w;

		character = text[i++];
	}
}
void drawTextWrapped(char* text, SDL_Renderer* renderer, SDL_Rect box, int r, int g, int b, int fontType){
	
	int i, character;
	SDL_Rect *glyph, dest;

	SDL_SetTextureColorMod(fontTexture, r, g, b);

	dest.x = box.x; 
	dest.y = box.y;
	i = 0;
	character = text[i++];
	while(character){
		glyph = &glyphs[character];

		dest.w = glyph->w;
		dest.h = glyph->h;
		if(dest.x + dest.w > box.w){
			dest.y = dest.y + glyph->h;
			dest.x = box.x;
		}
		SDL_RenderCopy(renderer, fontTexture, glyph, &dest);
		dest.x += glyph->w;
		character = text[i++];
	}
}

int getCharHeight(unsigned char character, int fontType){
	return glyphs[character].h;
}
int getCharWidth(unsigned char character,  int fontType){
	return glyphs[character].w;
}