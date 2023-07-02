#pragma once

#include "../effects/visualeffect.h"
#include <functional>
#include <string>

struct _TTF_Font;
using TTF_Font = _TTF_Font;

#include <SDL2/SDL_surface.h>

namespace e172::impl::sdl::spm {

template<typename... Args>
void withBlit(void (*f)(SDL_Surface *, Args...), SDL_Surface *surface, Args... args)
{
    auto tmp = SDL_CreateRGBSurface(0,
                                    surface->w,
                                    surface->h,
                                    32,
                                    0x00ff0000,
                                    0x0000ff00,
                                    0x000000ff,
                                    0xff000000);
    SDL_LockSurface(tmp);
    f(tmp, args...);
    SDL_UnlockSurface(tmp);
    SDL_UnlockSurface(surface);

    SDL_BlitSurface(tmp, nullptr, surface, nullptr);
    SDL_FreeSurface(tmp);
}

Uint32 ColorRGB(Uint8 R, Uint8 G, Uint8 B);

void fillPixel(SDL_Surface *surface, int x, int y, Uint32 color);
Uint32 pixelAt(SDL_Surface *surface, int x, int y);

SDL_Surface *createARGB32Surface(int width, int height);
SDL_Surface *createRGBA32Surface(int width, int height);
SDL_Surface *createABGR32Surface(int width, int height);

void applyEffect(VisualEffect *effect);
void lockEffect(VisualEffect *effect);
void unlockEffect();

void line(SDL_Surface *surface, int point1_x, int point1_y, int point2_x, int point2_y, Uint32 color);

void verticalLine(SDL_Surface *surface, int point_x, int point_y, int l, Uint32 color);

void horizontalLine(SDL_Surface *surface, int point_x, int point_y, int l, Uint32 color);

void square(SDL_Surface *surface, int point_x, int point_y, int l, Uint32 color);

void fillSquare(SDL_Surface *surface, int point_x, int point_y, int l, Uint32 color);

void rect(SDL_Surface *surface, int point1_x, int point1_y, int point2_x, int point2_y, Uint32 color);

void fillArea(
    SDL_Surface *surface, int point1_x, int point1_y, int point2_x, int point2_y, Uint32 color);

void circle(SDL_Surface *surface, int point1_x, int point1_y, int radius, Uint32 color);
void grid(SDL_Surface *surface,
          int point1_x,
          int point1_y,
          int point2_x,
          int point2_y,
          int interval,
          Uint32 color);

void diagonalGrid(SDL_Surface *surface,
                  int point1_x,
                  int point1_y,
                  int point2_x,
                  int point2_y,
                  int interval,
                  Uint32 color);

void blitRotatedSurface(SDL_Surface *surface,
                        SDL_Surface *screen_surface,
                        int x,
                        int y,
                        double angle,
                        double zoom,
                        int smooth,
                        VisualEffect *effect = nullptr);

void blendedText(SDL_Surface *surface,
                 std::string text_line,
                 TTF_Font *text_font,
                 int text_x,
                 int text_y,
                 SDL_Color text_color,
                 VisualEffect *effect = nullptr);

void blendedText(SDL_Surface *surface,
                 std::string text_line,
                 TTF_Font *text_font,
                 int text_x,
                 int text_y,
                 Uint32 color,
                 Uint32 wrap = 1024,
                 VisualEffect *effect = nullptr);

SDL_Surface *cutOutSurface(const SDL_Surface *surface, int x, int y, int w, int h);
SDL_Surface *copySurface(const SDL_Surface *surface);
SDL_Surface *flip(SDL_Surface *surface, bool xFlip, bool yFlip);

}; // namespace e172::impl::sdl::spm
