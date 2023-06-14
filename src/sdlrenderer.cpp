#include "sdlrenderer.h"
#include "spm.h"


#include <src/effects/anaglyph.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <src/math/math.h>
#include <src/debug.h>

const int SDLRenderer::DefaultFontSize = 20;

void SDLRenderer::enableEffect(uint64_t effect) {
    (void)effect;
    if(effect == 0) {
        m_anaglyphEnabled = true;
    } else {
        m_anaglyphEnabled2 = true;
    }
}

void SDLRenderer::disableEffect(uint64_t effect) {
    (void)effect;
    if(effect == 0) {
        m_anaglyphEnabled = false;
    } else {
        m_anaglyphEnabled2 = false;
    }
}

SDLRenderer::~SDLRenderer() {
    SDL_FreeSurface(m_surface);
    SDL_DestroyWindow(m_window);
}

void SDLRenderer::setResolution(e172::Vector<double> value)
{
    if (value.x() >= 0 && value.y() >= 0) {
        SDL_SetWindowSize(m_window, value.intX(), value.intY());
        m_surface = SDL_GetWindowSurface(m_window);
        m_resolution = value.into<uint32_t>();
    }
}

void SDLRenderer::applyLensEffect(const e172::Vector<double> &point0,
                                  const e172::Vector<double> &point1,
                                  double coefficient)
{
    const auto delta = point1 - point0;
    if (delta.x() == 0 || delta.y() == 0 || e172::Math::cmpf(coefficient, 0))
        return;

    m_lensQueue.push({ point0, point1, coefficient });
}

SDLRenderer::SDLRenderer(const std::string &title, const e172::Vector<uint32_t> &resolution)
{
    if (!s_sdlInitialized) {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();
        s_sdlInitialized = true;
    }
    SDL_Window *window = SDL_CreateWindow(title.c_str(),
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          resolution.x(),
                                          resolution.y(),
                                          0);

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    m_window = window;
    m_resolution = resolution;
    m_surface = surface;
}

void SDLRenderer::applyLensEffect(SDL_Surface *surface,
                                  const e172::Vector<double> point0,
                                  const e172::Vector<double> point1,
                                  double coef)
{
    auto pixels = reinterpret_cast<uint32_t*>(surface->pixels);

    auto center = (point0 + point1) / 2;

    uint32_t pix[point1.intY()-point0.intY()][point1.intX()-point0.intX()];
    for(int y = point0.intY(); y < point1.intY(); ++y) {
        for(int x = point0.intX(); x < point1.intX(); ++x) {
            uint32_t p = pixels[(y * surface->w) + x];

            const double mx = (M_PI) / (point1.x() - point0.x());
            const double my = (M_PI) / (point1.y() - point0.y());

            const int newY = static_cast<int>((y - center.y()) + coef * sin((y - center.y()) * my)
                                              + center.y());
            const int newX = static_cast<int>((x - center.x()) + coef * sin((x - center.x()) * mx)
                                              + center.x());

            if (((point0.intY() <= newY)&&(newY < point1.intY()))&&((point0.intX() <= newX)&&(newX < point1.intX()))){
                pix[newY-point0.intY()][newX-point0.intX()] = p;
            }
        }
    }

    for (int y = point0.intY(); y < point1.intY(); ++y) {
        for(int x = point0.intX(); x < point1.intX(); ++x) {
            SPM::FillPixel(surface, x, y, pix[y-point0.intY()][x-point0.intX()]);
        }
    }
}

size_t SDLRenderer::presentEffectCount() const {
    return 0;
}

std::string SDLRenderer::presentEffectName(size_t index) const {
    return "";
}

void SDLRenderer::drawEffect(size_t index, const e172::VariantVector &args) {

}

void SDLRenderer::fill(uint32_t color) {
    m_drawQueue.push(m_depth, [this, color]() { SDL_FillRect(m_surface, nullptr, color); });
}

e172::Vector<double> SDLRenderer::screenSize() const
{
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    return e172::Vector<double>(displayMode.w, displayMode.h);
}

void SDLRenderer::setFullscreen(bool value) {
    if(m_lastFullscreen == value)
        return;

    m_lastFullscreen = value;

    if(value) {
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);
        setResolution(e172::Vector<double>(displayMode.w, displayMode.h));

        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
        SDL_ShowCursor(false);
    } else {
        SDL_SetWindowFullscreen(m_window, 0);
        SDL_ShowCursor(true);
    }
    if (m_surface)
        SDL_FreeSurface(m_surface);

    m_surface = SDL_GetWindowSurface(m_window);
}

void SDLRenderer::drawPixel(const e172::Vector<double> &point, Uint32 color)
{
    m_drawQueue.push(m_depth, [this, point, color]() {
        SPM::FillPixel(m_surface, point.intX(), point.intY(), color);
    });
}

void SDLRenderer::drawLine(const e172::Vector<double> &point0,
                           const e172::Vector<double> &point1,
                           Uint32 color)
{
    m_drawQueue.push(m_depth, [this, point0, point1, color]() {
        SDL_LockSurface(m_surface);
        int x0 = point0.intX();
        int y0 = point0.intY();
        int x1 = point1.intX();
        int y1 = point1.intY();
        if(x0 == x1) {
            if(y0 < 0) {
                y0 = 0;
            } else if(y0 > m_resolution.intY()) {
                y0 = m_resolution.intY();
            }
            if(y1 < 0) {
                y1 = 0;
            } else if(y1 > m_resolution.intY()) {
                y1 = m_resolution.intY();
            }
        } else {
            double k = static_cast<double>(y1 - y0) / static_cast<double>(x1 - x0);
            double b = y0 - k * x0;

            if(x0 < 0) {
                x0 = 0;
                y0 = k * x0 + b;
            } else if(x0 > m_resolution.intX()) {
                x0 = m_resolution.intX();
                y0 = k * x0 + b;
            }
            if(x1 < 0) {
                x1 = 0;
                y1 = k * x1 + b;
            } else if(x1 > m_resolution.intX()) {
                x1 = m_resolution.intX();
                y1 = k * x1 + b;
            }
        }
        SPM::Line(m_surface, x0, y0, x1, y1, color);
        SDL_UnlockSurface(m_surface);
    });
}

void SDLRenderer::drawRect(const e172::Vector<double> &point0,
                           const e172::Vector<double> &point1,
                           Uint32 color,
                           const e172::ShapeFormat &format)
{
    m_drawQueue.push(m_depth, [this, point0, point1, color, format]() {
        SDL_LockSurface(m_surface);
        if(format.fill()) {
            SPM::WithBlit(SPM::FillArea,
                          m_surface,
                          point0.intX(),
                          point0.intY(),
                          point1.intX(),
                          point1.intY(),
                          color);
        } else {
            SPM::Rect(m_surface, point0.intX(), point0.intY(), point1.intX(), point1.intY(), color);
        }
        SDL_UnlockSurface(m_surface);
    });
}

void SDLRenderer::drawSquare(const e172::Vector<double> &point, int radius, Uint32 color)
{
    m_drawQueue.push(m_depth, [this, point, radius, color]() {
        SDL_LockSurface(m_surface);
        SPM::Square(m_surface, point.intX(), point.intY(), radius, color);
        SDL_UnlockSurface(m_surface);
    });
}

void SDLRenderer::drawCircle(const e172::Vector<double> &point, int radius, Uint32 color)
{
    m_drawQueue.push(m_depth, [this, point, radius, color]() {
        SDL_LockSurface(m_surface);
        SPM::Circle(m_surface, point.intX(), point.intY(), radius, color);
        SDL_UnlockSurface(m_surface);
    });
}

void SDLRenderer::drawDiagonalGrid(const e172::Vector<double> &point1,
                                   const e172::Vector<double> &point2,
                                   int interval,
                                   Uint32 color)
{
    m_drawQueue.push(m_depth, [this, point1, point2, interval, color]() {
        SDL_LockSurface(m_surface);
        SPM::DiagonalGrid(m_surface,
                          point1.intX(),
                          point1.intY(),
                          point2.intX(),
                          point2.intY(),
                          interval,
                          color);
        SDL_UnlockSurface(m_surface);
    });
}

void SDLRenderer::drawImage(const e172::Image &image,
                            const e172::Vector<double> &pos,
                            double angle,
                            double zoom)
{
    m_drawQueue.push(m_depth, [this, image, pos, angle, zoom](){
        if(imageProvider(image) == provider()) {
            VisualEffect *effect = nullptr;
            if (m_anaglyphEnabled || m_anaglyphEnabled2)
                effect = new Anaglyph(e172::Vector(2, 1));

            auto image_surface = imageData<SDL_Surface*>(image);
            SPM::BlitRotatedSurface(image_surface,
                                    m_surface,
                                    pos.intX(),
                                    pos.intY(),
                                    angle,
                                    zoom,
                                    1,
                                    effect);
            delete effect;
        }
    });
}

e172::Vector<double> SDLRenderer::drawString(const std::string &string,
                                             const e172::Vector<double> &pos,
                                             uint32_t color,
                                             const e172::TextFormat &format)
{
    int expectedSize = DefaultFontSize;
    if(format.fontSize() > 0) {
        expectedSize = format.fontSize();
    }

    const auto font = m_fonts.find(format.font());
    if(font != m_fonts.end()) {    
        TTF_Font *f = nullptr;
        const auto s = font->second.sizes.find(expectedSize);
        if(s != font->second.sizes.end()) {
            f = s->second;
        } else {
            f = TTF_OpenFont(font->second.path.c_str(), expectedSize);
            if(f) {
                font->second.sizes[expectedSize] = f;
            } else {
                throw std::runtime_error("can not load font: " + font->second.path);
            }
        }

        VisualEffect *effect = nullptr;
        if (m_anaglyphEnabled || m_anaglyphEnabled2)
            effect = new Anaglyph(e172::Vector(2, 1));

        int w = 0, h = 0;
        TTF_SizeText(f, string.c_str(), &w, &h);
        int offsetX = 0, offsetY = 0;
        if(format.alignment() & e172::TextFormat::AlignHCenter) {
            offsetX = -w / 2;
        }
        if(format.alignment() & e172::TextFormat::AlignVCenter) {
            offsetY = -h / 2;
        }
        m_drawQueue.push(m_depth, [this, string, f, pos, offsetX, offsetY, effect, color]() {
            SPM::BlendedText(m_surface,
                             string,
                             f,
                             pos.intX() + offsetX,
                             pos.intY() + offsetY,
                             color,
                             1024,
                             effect);
            delete effect;
        });
        return e172::Vector<double>(w, h);
    }
    return e172::Vector<double>();
}

void SDLRenderer::modifyBitmap(const std::function<void(e172::Color *)> &modifier)
{
    m_drawQueue.push(m_depth, [this, modifier]() {
        SDL_LockSurface(m_surface);
        modifier(reinterpret_cast<e172::Color *>(m_surface->pixels));
        SDL_UnlockSurface(m_surface);
    });
}

bool SDLRenderer::update() {
    m_drawQueue.exec();

    while (m_lensQueue.size() > 0) {
        const auto l = m_lensQueue.front();
        applyLensEffect(m_surface, l.point0, l.point1, l.coeficient);
        m_lensQueue.pop();
    }

    SDL_UpdateWindowSurface(m_window);
    return true;
}


void SDLRenderer::applySmooth(const e172::Vector<double> &point0, const e172::Vector<double> &point1, double coefficient) {
    uint32_t pix[point1.intY()-point0.intY()][point1.intX()-point0.intX()];
    for(int y = point0.intY(); y < point1.intY(); ++y) {
        for(int x = point0.intX(); x < point1.intX(); ++x) {
            pix[y - point0.intY()][x - point0.intX()]
                = (SPM::GetPixel(m_surface, static_cast<int>(x + coefficient), y)
                   + SPM::GetPixel(m_surface, static_cast<int>(x - coefficient), y)
                   + SPM::GetPixel(m_surface, x, static_cast<int>(y + coefficient))
                   + SPM::GetPixel(m_surface, x, static_cast<int>(y - coefficient)))
                  / 4;
        }
    }

    for(int y = point0.intY(); y < point1.intY(); ++y) {
        for(int x = point0.intX(); x < point1.intX(); ++x) {
            SPM::FillPixel(m_surface, x, y, pix[y - point0.intY()][x - point0.intX()]);
        }
    }
}

void SDLRenderer::setDepth(int64_t depth) {
    m_depth = depth;
}
