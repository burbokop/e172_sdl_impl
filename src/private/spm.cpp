#include "spm.h"

#include "rotozoom.h"
#include <SDL2/SDL_ttf.h>

#define SPM_PI 3.141592653589793238462643383279502884
#define RADS_MULTIPLIER -180 / SPM_PI

namespace e172::impl::sdl::spm {

Uint32 colorRGB(Uint8 R, Uint8 G, Uint8 B)
{
    return 65536 * R + 256 * G + B;
}

void fillPixel(SDL_Surface *surface, int x, int y, Uint32 color)
{
    Uint32 *pixels = static_cast<Uint32*>(surface->pixels);
    if (x >= 0 && y >= 0 && x < surface->w && y < surface->h) {
        pixels[(y * surface->w) + x] = color;
    }
}

Uint32 pixelAt(SDL_Surface *surface, int x, int y)
{
    if (x >= 0 && y >= 0 && x < surface->w && y < surface->h) {
        return (static_cast<Uint32*>(surface->pixels))[x + surface->w * y];
    }
    return 0;
}

void line(SDL_Surface *surface, int point1_x, int point1_y, int point2_x, int point2_y, Uint32 color)
{
    int d, dL, dU, dx, dy, temp;
    dx = point2_x - point1_x;
    dy = point2_y - point1_y;
    if (dy < 0)
    {
        temp = point2_x;
        point2_x = point1_x;
        point1_x = temp;
        temp = point2_y;
        point2_y = point1_y;
        point1_y = temp;
    }
    dx = point2_x - point1_x;
    dy = point2_y - point1_y;
    if (dx >= 0)
    {
        if (dy > dx)
        {
            d = dy - 2 * dx;
            dL = -2 * dx;
            dU = 2 * dy - 2 * dx;
            for(int x = point1_x, y = point1_y; y <= point2_y; y++)
            {
                fillPixel(surface, x, y, color);
                if (d >= 1)
                {
                    d += dL;
                }
                else
                {
                    x++;
                    d += dU;
                }
            }
        }
        else
        {
            d = 2 * dy - dx;
            dL = 2 * dy;
            dU = 2 * dy - 2 * dx;
            for(int x = point1_x, y = point1_y; x <= point2_x; x++)
            {
                fillPixel(surface, x, y, color);
                if (d <= 0)
                {
                    d += dL;
                }
                else
                {
                    y++;
                    d += dU;
                }
            }
        }
    }
    else if (dx < 0)
    {
        if (dy > -dx)
        {
            d = dy - 2 * -dx;
            dL = -2 * -dx;
            dU = 2 * dy - 2 * -dx;
            for (int x = point1_x, y = point1_y; y <= point2_y; y++)
            {
                fillPixel(surface, x, y, color);
                if (d >= 1)
                {
                    d += dL;
                }
                else
                {
                    --x;
                    d += dU;
                }
            }
        }
        else
        {
            d = 2 * dy + dx;
            dL = 2 * dy;
            dU = 2 * dy - 2 * -dx;
            for (int x = point1_x, y = point1_y; x >= point2_x; --x)
            {
                fillPixel(surface, x, y, color);
                if (d <= 0)
                {
                    d += dL;
                }
                else
                {
                    y++;
                    d += dU;
                }
            }
        }
    }
}

void verticalLine(SDL_Surface *surface, int point_x, int point_y, int l, Uint32 color)
{
    for (int i = 0; i < l; i++) {
        fillPixel(surface, point_x, point_y + i, color);
    }
}

void horizontalLine(SDL_Surface *surface, int point_x, int point_y, int l, Uint32 color)
{
    for (int i = 0; i < l; i++) {
        fillPixel(surface, point_x + i, point_y, color);
    }
}

void square(SDL_Surface *surface, int point_x, int point_y, int l, Uint32 color)
{
    point_x -= l;
    point_y -= l;
    l *= 2;

    for(int i = 0; i < l; i++)
    {
        fillPixel(surface, point_x + i, point_y, color);
        fillPixel(surface, point_x + i, point_y + l, color);
        fillPixel(surface, point_x, point_y + i, color);
        fillPixel(surface, point_x + l, point_y + i, color);
    }
    fillPixel(surface, point_x + l, point_y + l, color);
}

void fillSquare(SDL_Surface *surface, int point_x, int point_y, int l, Uint32 color)
{
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < l; j++)
        {
            fillPixel(surface, point_x + j, point_y + i, color);
        }
    }
}

void rect(SDL_Surface *surface, int point1_x, int point1_y, int point2_x, int point2_y, Uint32 color)
{
    int dx = point2_x - point1_x, dy = point2_y - point1_y;
    if (dx >= 0)
    {
        for (int i = 0; i <= dx; i++)
        {
            fillPixel(surface, point1_x + i, point1_y, color);
            fillPixel(surface, point1_x + i, point1_y + dy, color);
        }
    }
    else if (dx < 0)
    {
        for (int i = 0; i >= dx; i--)
        {
            fillPixel(surface, point1_x + i, point1_y, color);
            fillPixel(surface, point1_x + i, point1_y + dy, color);
        }
    }
    if (dy >= 0)
    {
        for (int i = 0; i <= dy; i++)
        {
            fillPixel(surface, point1_x, point1_y + i, color);
            fillPixel(surface, point1_x + dx, point1_y + i, color);
        }
    }
    else if (dy < 0)
    {
        for (int i = 0; i >= dy; i--)
        {
            fillPixel(surface, point1_x, point1_y + i, color);
            fillPixel(surface, point1_x + dx, point1_y + i, color);
        }
    }
}

void fillArea(
    SDL_Surface *surface, int point1_x, int point1_y, int point2_x, int point2_y, Uint32 color)
{
    int dx = point2_x - point1_x, dy = point2_y - point1_y;
    if (dx >= 0)
    {
        if (dy >= 0)
        {
            for (int i = 0; i < dy; i++)
            {
                for (int j = 0; j < dx; j++)
                {
                    fillPixel(surface, point1_x + j, point1_y + i, color);
                }
            }
        }
        else
        {
            for (int i = 0; i >= dy; i--)
            {
                for (int j = 0; j < dx; j++)
                {
                    fillPixel(surface, point1_x + j, point1_y + i, color);
                }
            }
        }
    }
    else
    {
        if (dy >= 0)
        {
            for (int i = 0; i < dy; i++)
            {
                for (int j = 0; j >= dx; j--)
                {
                    fillPixel(surface, point1_x + j, point1_y + i, color);
                }
            }
        }
        else
        {
            for (int i = 0; i >= dy; i--)
            {
                for (int j = 0; j >= dx; j--)
                {
                    fillPixel(surface, point1_x + j, point1_y + i, color);
                }
            }
        }
    }
}

void circle(SDL_Surface *surface, int center_x, int center_y, int radius, Uint32 color)
{
    for(int i = 0; i < sqrt(radius * radius - i * i); i++) {
        int i2 = static_cast<int>(sqrt(radius * radius - i * i));
        fillPixel(surface, center_x + i, center_y + i2, color);
        fillPixel(surface, center_x - i, center_y - i2, color);
        fillPixel(surface, center_x + i, center_y - i2, color);
        fillPixel(surface, center_x - i, center_y + i2, color);

        fillPixel(surface, center_x + i2, center_y + i, color);
        fillPixel(surface, center_x - i2, center_y - i, color);
        fillPixel(surface, center_x - i2, center_y + i, color);
        fillPixel(surface, center_x + i2, center_y - i, color);
    }
}

void grid(SDL_Surface *surface,
          int point1_x,
          int point1_y,
          int point2_x,
          int point2_y,
          int interval,
          Uint32 color)
{
    for (int i = 0; i < std::abs(point2_x - point1_x) / interval; i++)
    {
        for (int j = 0; j < std::abs(point2_y - point1_y); j++)
        {
            fillPixel(surface, point1_x + (i * interval), point1_y + j, color);
        }
    }
    for (int i = 0; i < std::abs(point2_y - point1_y) / interval; i++)
    {
        for (int j = 0; j < std::abs(point2_x - point1_x); j++)
        {
            fillPixel(surface, point1_x + j, point1_y + (i * interval), color);
        }
    }
}

void diagonalGrid(SDL_Surface *surface,
                  int point1_x,
                  int point1_y,
                  int point2_x,
                  int point2_y,
                  int interval,
                  Uint32 color)
{
    SDL_Surface *canvas = createRGBA32Surface(std::abs(point2_x - point1_x),
                                              std::abs(point2_y - point1_y));
    for (int i = 0, L = std::abs(point2_x - point1_x) / (point2_y - point1_y) * interval + interval; i < L; i++) {
        line(canvas,
             canvas->h * (i - interval / 2) / interval,
             0,
             canvas->h * (i + interval / 2) / interval,
             canvas->h,
             color);
    }
    SDL_BlitSurface(canvas, nullptr, surface, new SDL_Rect { point1_x, point1_y, 0, 0 });
    SDL_FreeSurface(canvas);
}

void blitRotatedSurface(SDL_Surface *surface,
                        SDL_Surface *screen_surface,
                        int centerX,
                        int centerY,
                        double angle,
                        double zoom,
                        int smooth,
                        VisualEffect *effect)
{
    SDL_Surface *temp_surface = nullptr;
    SDL_Rect rect = {static_cast<int>(centerX - surface->w * zoom / 2),
                     static_cast<int>(centerY - surface->h * zoom / 2),
                     0,
                     0};
    angle = angle * RADS_MULTIPLIER;
    temp_surface = rotozoomSurface(surface, angle, zoom, smooth);
    rect.x = static_cast<int>(rect.x - temp_surface->w / 2 + surface->w * zoom / 2);
    rect.y = static_cast<int>(rect.y - temp_surface->h / 2 + surface->h * zoom / 2);

    SDL_Surface *ets = nullptr;
    if(effect) {
        ets = (*effect)(temp_surface);
    } else {
        ets = temp_surface;
    }

    SDL_BlitSurface(ets, nullptr, screen_surface, &rect);
    if(ets != temp_surface)
        SDL_FreeSurface(ets);
    SDL_FreeSurface(temp_surface);
}

void blendedText(SDL_Surface *surface,
                 std::string text_line,
                 TTF_Font *text_font,
                 int text_x,
                 int text_y,
                 SDL_Color text_color,
                 VisualEffect *effect)
{
    SDL_Surface *text_surface = nullptr;
    SDL_Rect text_rect;
    text_surface = TTF_RenderText_Blended(text_font, text_line.c_str(), text_color);
    text_rect.x = text_x;
    text_rect.y = text_y;

    SDL_Surface *ets = nullptr;
    if(effect) {
        ets = (*effect)(text_surface);
    } else {
        ets = text_surface;
    }

    SDL_BlitSurface(ets, nullptr, surface, &text_rect);
    if(ets != text_surface)
        SDL_FreeSurface(ets);
    SDL_FreeSurface(text_surface);
}

void blendedText(SDL_Surface *surface,
                 std::string text_line,
                 TTF_Font *text_font,
                 int text_x,
                 int text_y,
                 Uint32 color,
                 Uint32 wrap,
                 VisualEffect *effect)
{
    SDL_Surface *text_surface = nullptr;
    SDL_Rect text_rect;
    SDL_Color rgbColor;
    rgbColor.a = (color >> 24) & 255;
    rgbColor.r = (color >> 16) & 255;
    rgbColor.g = (color >> 8) & 255;
    rgbColor.b = (color >> 0) & 255;
    text_surface = TTF_RenderUTF8_Blended_Wrapped(text_font, text_line.c_str(), rgbColor, wrap);
    if(text_surface) {
        text_rect.x = text_x;
        text_rect.y = text_y;

        SDL_Surface *ets = nullptr;
        if(effect) {
            ets = (*effect)(text_surface);
        } else {
            ets = text_surface;
        }

        SDL_BlitSurface(ets, nullptr, surface, &text_rect);
        if(ets != text_surface)
            SDL_FreeSurface(ets);
        SDL_FreeSurface(text_surface);
    }
}

SDL_Surface *cutOutSurface(const SDL_Surface *surface, int x, int y, int w, int h)
{
    auto non_const_surface = const_cast<SDL_Surface*>(surface);

    SDL_Rect surfaceRect;
    surfaceRect.x = x;
    surfaceRect.y = y;
    surfaceRect.w = w;
    surfaceRect.h = h;
    SDL_Surface *tmp;
    tmp = SDL_CreateRGBSurface(0, surfaceRect.w, surfaceRect.h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    SDL_BlitSurface(non_const_surface, &surfaceRect, tmp, nullptr);
    return tmp;
}

SDL_Surface *copySurface(const SDL_Surface *surface)
{
    return SDL_CreateRGBSurfaceWithFormatFrom(surface->pixels, surface->w, surface->h, surface->format->BitsPerPixel, surface->pitch, surface->format->format);
    //return SDL_CreateRGBSurfaceFrom(surface->pixels, surface->w, surface->h, 32, surface->pitch, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}

SDL_Surface *flip(SDL_Surface *surface, bool xFlip, bool yFlip)
{
    SDL_Surface *result = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    Uint32 *pixels = static_cast<Uint32*>(surface->pixels);
    if(xFlip) {
        for (int y = 0; y < surface->h; ++y) {
            for (int x = 0; x < surface->w; ++x) {
                fillPixel(result, surface->w - x, y, pixels[x + surface->w * y]);
            }
        }
    }
    else if(yFlip) {
        for (int y = 0; y < surface->h; ++y) {
            for (int x = 0; x < surface->w; ++x) {
                fillPixel(result, x, surface->h - y, pixels[x + surface->w * y]);
            }
        }
    }
    else if(xFlip && yFlip) {
        for (int y = 0; y < surface->h; ++y) {
            for (int x = 0; x < surface->w; ++x) {
                fillPixel(result, surface->w - x, surface->h - y, pixels[x + surface->w * y]);
            }
        }
    }
    return result;
}

SDL_Surface *createARGB32Surface(int width, int height)
{
    return SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
}

SDL_Surface *createRGBA32Surface(int width, int height)
{
    return SDL_CreateRGBSurface(0, width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
}

SDL_Surface *createABGR32Surface(int width, int height)
{
    return SDL_CreateRGBSurface(0, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}

} // namespace e172::impl::sdl::spm
