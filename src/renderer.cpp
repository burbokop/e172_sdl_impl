#include "renderer.h"

#include "effects/anaglyph.h"
#include "graphicsprovider.h"
#include "private/spm.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <e172/debug.h>
#include <e172/math/math.h>

namespace e172::impl::sdl {

const int Renderer::DefaultFontSize = 20;

void Renderer::enableEffect(uint64_t effect)
{
    (void)effect;
    if(effect == 0) {
        m_anaglyphEnabled = true;
    } else {
        m_anaglyphEnabled2 = true;
    }
}

void Renderer::disableEffect(uint64_t effect)
{
    (void)effect;
    if(effect == 0) {
        m_anaglyphEnabled = false;
    } else {
        m_anaglyphEnabled2 = false;
    }
}

Renderer::~Renderer()
{
    SDL_FreeSurface(m_surface);
    SDL_DestroyWindow(m_window);
}

void Renderer::setResolution(const e172::Vector<uint32_t> &value)
{
    SDL_SetWindowSize(m_window, value.x(), value.y());
    if (m_surface) {
        SDL_FreeSurface(m_surface);
    }
    m_surface = SDL_GetWindowSurface(m_window);
    m_resolution = value.into<std::uint32_t>();
}

void Renderer::applyLensEffect(const e172::Vector<double> &point0,
                               const e172::Vector<double> &point1,
                               double coefficient)
{
    const auto delta = point1 - point0;
    if (delta.x() == 0 || delta.y() == 0 || e172::Math::cmpf(coefficient, 0))
        return;

    m_lensQueue.push({ point0, point1, coefficient });
}

Renderer::Renderer(Private, const std::string &title, const e172::Vector<uint32_t> &resolution)
{
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

void Renderer::applyLensEffect(SDL_Surface *surface,
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
            spm::fillPixel(surface, x, y, pix[y - point0.intY()][x - point0.intX()]);
        }
    }
}

size_t Renderer::presentEffectCount() const
{
    return 0;
}

std::string Renderer::presentEffectName(std::size_t) const
{
    return "";
}

void Renderer::drawEffect(std::size_t, const e172::VariantVector &) {}

void Renderer::fill(Color color)
{
    m_drawQueue.push(m_depth, [this, color]() { SDL_FillRect(m_surface, nullptr, color); });
}

void Renderer::setFullscreen(bool value)
{
    if(m_lastFullscreen == value)
        return;

    m_lastFullscreen = value;

    if(value) {
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);
        assert(displayMode.w >= 0);
        assert(displayMode.h >= 0);
        setResolution(e172::Vector<std::uint32_t>(displayMode.w, displayMode.h));

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

void Renderer::drawPixel(const e172::Vector<double> &point, Uint32 color)
{
    m_drawQueue.push(m_depth, [this, point, color]() {
        spm::fillPixel(m_surface, point.intX(), point.intY(), color);
    });
}

void Renderer::drawLine(const e172::Vector<double> &point0,
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
        spm::line(m_surface, x0, y0, x1, y1, color);
        SDL_UnlockSurface(m_surface);
    });
}

void Renderer::drawRect(const e172::Vector<double> &point0,
                        const e172::Vector<double> &point1,
                        Uint32 color,
                        const e172::ShapeFormat &format)
{
    m_drawQueue.push(m_depth, [this, point0, point1, color, format]() {
        SDL_LockSurface(m_surface);
        if(format.fill()) {
            spm::withBlit(spm::fillArea,
                          m_surface,
                          point0.intX(),
                          point0.intY(),
                          point1.intX(),
                          point1.intY(),
                          color);
        } else {
            spm::rect(m_surface, point0.intX(), point0.intY(), point1.intX(), point1.intY(), color);
        }
        SDL_UnlockSurface(m_surface);
    });
}

void Renderer::drawSquare(const e172::Vector<double> &point, double radius, Color color)
{
    m_drawQueue.push(m_depth, [this, point, radius, color]() {
        SDL_LockSurface(m_surface);
        spm::square(m_surface, point.intX(), point.intY(), radius, color);
        SDL_UnlockSurface(m_surface);
    });
}

void Renderer::drawCircle(const e172::Vector<double> &point, double radius, Color color)
{
    m_drawQueue.push(m_depth, [this, point, radius, color]() {
        SDL_LockSurface(m_surface);
        spm::circle(m_surface, point.intX(), point.intY(), radius, color);
        SDL_UnlockSurface(m_surface);
    });
}

void Renderer::drawDiagonalGrid(const e172::Vector<double> &point1,
                                const e172::Vector<double> &point2,
                                double interval,
                                Color color)
{
    m_drawQueue.push(m_depth, [this, point1, point2, interval, color]() {
        SDL_LockSurface(m_surface);
        spm::diagonalGrid(m_surface,
                          point1.intX(),
                          point1.intY(),
                          point2.intX(),
                          point2.intY(),
                          interval,
                          color);
        SDL_UnlockSurface(m_surface);
    });
}

void Renderer::drawImage(const e172::Image &image,
                         const e172::Vector<double> &center,
                         double angle,
                         double zoom)
{
    m_drawQueue.push(m_depth, [this, image, center, angle, zoom]() {
        if(imageProvider(image) == provider()) {
            VisualEffect *effect = nullptr;
            if (m_anaglyphEnabled || m_anaglyphEnabled2)
                effect = new Anaglyph(e172::Vector(2, 1));

            auto image_surface = imageData<SDL_Surface*>(image);
            spm::blitRotatedSurface(image_surface,
                                    m_surface,
                                    center.intX(),
                                    center.intY(),
                                    angle,
                                    zoom,
                                    1,
                                    effect);
            delete effect;
        }
    });
}

e172::Vector<double> Renderer::drawString(const std::string &string,
                                          const e172::Vector<double> &pos,
                                          Color color,
                                          const e172::TextFormat &format)
{
    int expectedSize = DefaultFontSize;
    if(format.fontSize() > 0) {
        expectedSize = format.fontSize();
    }

    const auto prov = const_cast<GraphicsProvider *>(
        dynamic_cast<const GraphicsProvider *>(provider()));

    const auto font = prov->m_fonts.find(format.font());
    if (font != prov->m_fonts.end()) {
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
            spm::blendedText(m_surface,
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

void Renderer::modifyBitmap(const std::function<void(e172::Color *)> &modifier)
{
    m_drawQueue.push(m_depth, [this, modifier]() {
        SDL_LockSurface(m_surface);
        modifier(reinterpret_cast<e172::Color *>(m_surface->pixels));
        SDL_UnlockSurface(m_surface);
    });
}

bool Renderer::update()
{
    m_drawQueue.exec();

    while (m_lensQueue.size() > 0) {
        const auto l = m_lensQueue.front();
        applyLensEffect(m_surface, l.point0, l.point1, l.coeficient);
        m_lensQueue.pop();
    }

    SDL_UpdateWindowSurface(m_window);
    return true;
}

void Renderer::applySmooth(const e172::Vector<double> &point0,
                           const e172::Vector<double> &point1,
                           double coefficient)
{
    uint32_t pix[point1.intY()-point0.intY()][point1.intX()-point0.intX()];
    for(int y = point0.intY(); y < point1.intY(); ++y) {
        for(int x = point0.intX(); x < point1.intX(); ++x) {
            pix[y - point0.intY()][x - point0.intX()]
                = (spm::pixelAt(m_surface, static_cast<int>(x + coefficient), y)
                   + spm::pixelAt(m_surface, static_cast<int>(x - coefficient), y)
                   + spm::pixelAt(m_surface, x, static_cast<int>(y + coefficient))
                   + spm::pixelAt(m_surface, x, static_cast<int>(y - coefficient)))
                  / 4;
        }
    }

    for(int y = point0.intY(); y < point1.intY(); ++y) {
        for(int x = point0.intX(); x < point1.intX(); ++x) {
            spm::fillPixel(m_surface, x, y, pix[y - point0.intY()][x - point0.intX()]);
        }
    }
}

void Renderer::setDepth(int64_t depth)
{
    m_depth = depth;
}

} // namespace e172::impl::sdl
