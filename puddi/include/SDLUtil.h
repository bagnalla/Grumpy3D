#ifndef SDLUTIL_H
#define SDLUTIL_H

#include <SDL2/SDL.h>

namespace puddi
{
	namespace SDLUtil
	{
		void setPixel(SDL_Surface *screen, int x, int y, Uint32 newcolor);

		Uint32 getPixel(SDL_Surface *surface, int x, int y);

		// DOESN'T WORK ATM FOR SOME REASON
		// in the example at http://www.gamedev.net/topic/308570-sdl-mirror-surface/
		// the first argument SDL_CreateRGBSurface is SDL_HWSURFACE
		//SDL_Surface* mirrorSurfaceX(SDL_Surface *screen);

		SDL_Surface* mirrorSurfaceY(SDL_Surface *screen);

		void mirrorSurfaceX(SDL_Surface *surface);
	}
}

#endif
