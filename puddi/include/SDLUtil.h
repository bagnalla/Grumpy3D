#ifndef SDLUTIL_H
#define SDLUTIL_H

#include <SDL2/SDL.h>

namespace puddi
{
	class SDLUtil
	{
	public:
		static void setPixel(SDL_Surface *screen, int x, int y, Uint32 newcolor);

		static Uint32 getPixel(SDL_Surface *surface, int x, int y);

		// DOESN'T WORK ATM FOR SOME REASON
		// in the example at http://www.gamedev.net/topic/308570-sdl-mirror-surface/
		// the first argument SDL_CreateRGBSurface is SDL_HWSURFACE
		//static SDL_Surface* mirrorSurfaceX(SDL_Surface *screen);

		static SDL_Surface* mirrorSurfaceY(SDL_Surface *screen);

		static void mirrorSurfaceX(SDL_Surface *surface);
	};
}

#endif
