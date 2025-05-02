#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sdlcalls.h"

typedef struct {} demoDraw_Context;

extern SDL_Window *SDLwindow;
SDL_Renderer *gRenderer = NULL; // Global renderer (initialize once)
TTF_Font *gFont = NULL;       // Global font (load once)

SDL_Color convertColor(unsigned int color) {
    SDL_Color sdlColor;

    // Extract the Red component
    sdlColor.r = (color >> 0x18);

    // Extract the Green component
    sdlColor.g = (color >> 0x10 & 0xFF);

    // Extract the Blue component
    sdlColor.b = (color >> 8 & 0xFF);

    // Assuming the most significant byte (alpha) should default to fully opaque (255)
    sdlColor.a = 0;

    return sdlColor;
}

void drawText(demoDraw_Context* context, int posX, int posY, unsigned int color, char* text)
{
	if (strcmp(text, "") == 0){
		text = " ";
	}
	if (TTF_Init() != 0)
    {
        printf("Failed to init SDL_TTF.TTF Error: %s\n", TTF_GetError());
        exit(1);
    }
	if (!gRenderer){
		printf("Initializing Renderer");
		gRenderer = SDL_CreateRenderer(SDLwindow, -1, SDL_RENDERER_ACCELERATED);
		if (!gRenderer){
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        	exit(1);
		}
	}
	if (!gFont){
		printf("Initializing Font");
		gFont = TTF_OpenFont("/usr/share/fonts/dejavu/DejaVuSansMono.ttf", 11);
		if (!gFont){
			printf("Failed to load font! TTF Error: %s\n", TTF_GetError());
        	exit(1);
		}
	}

    SDL_Color textColor = convertColor(color);
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text, textColor);
    if (textSurface == NULL) {
        printf("Failed to render text surface! TTF Error: %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (textTexture == NULL) {
        printf("Failed to create texture from surface! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_Rect charRect;
    charRect.x = posX;
    charRect.y = posY;
    SDL_QueryTexture(textTexture, NULL, NULL, &charRect.w, &charRect.h); // Get actual width and height
    if (SDL_RenderCopy(gRenderer, textTexture, NULL, &charRect) != 0) {
        printf("Failed to render texture on SDL2! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_DestroyTexture(textTexture); // Destroy the texture after rendering
    return;
}