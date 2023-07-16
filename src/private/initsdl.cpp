#include "initsdl.h"

#include <SDL2/SDL_ttf.h>

void e172::impl::sdl::initSDL()
{
    static auto initialized = false;
    if (!initialized) {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();
        initialized = true;
    }
}
