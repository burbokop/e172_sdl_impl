#pragma once

#include <e172/graphics/abstractrenderer.h>
#include <e172/math/vector.h>
#include <e172/utility/priorityprocedure.h>
#include <map>
#include <queue>

struct SDL_Window;
struct SDL_Surface;
struct _TTF_Font;
typedef _TTF_Font TTF_Font;

namespace e172::impl::sdl {

class Renderer : public e172::AbstractRenderer
{
    friend class GraphicsProvider;

public:
    static const int DefaultFontSize;

    ~Renderer();

    // AbstractRenderer interface
public:
    void fill(uint32_t color) override;
    void drawPixel(const e172::Vector<double> &point, uint32_t color) override;
    void drawLine(const e172::Vector<double> &point0,
                  const e172::Vector<double> &point1,
                  uint32_t color) override;

    void drawRect(const e172::Vector<double> &point0,
                  const e172::Vector<double> &point1,
                  uint32_t color,
                  const e172::ShapeFormat &format = e172::ShapeFormat(false)) override;

    void drawSquare(const e172::Vector<double> &point, int radius, uint32_t color) override;
    void drawCircle(const e172::Vector<double> &point, int radius, uint32_t color) override;
    void drawDiagonalGrid(const e172::Vector<double> &point1,
                          const e172::Vector<double> &point2,
                          int interval,
                          uint32_t color) override;

    void drawImage(const e172::Image &image,
                   const e172::Vector<double> &pos,
                   double angle,
                   double zoom) override;

    e172::Vector<double> drawString(const std::string &string,
                                    const e172::Vector<double> &pos,
                                    uint32_t color,
                                    const e172::TextFormat &format = e172::TextFormat()) override;

    void modifyBitmap(const std::function<void(e172::Color *bitmap)> &modifier) override;

    void enableEffect(uint64_t effect) override;
    void disableEffect(uint64_t effect) override;

    e172::Vector<double> resolution() const override { return m_resolution.into<double>(); }
    void setResolution(e172::Vector<double> value) override;

    size_t presentEffectCount() const override;
    std::string presentEffectName(size_t index) const override;
    void drawEffect(size_t index, const e172::VariantVector &args) override;

    void applyLensEffect(const e172::Vector<double> &point0,
                         const e172::Vector<double> &point1,
                         double coefficient) override;

    void applySmooth(const e172::Vector<double> &point0,
                     const e172::Vector<double> &point1,
                     double coefficient) override;

    void setDepth(int64_t depth) override;

    void setFullscreen(bool value) override;
    e172::Vector<double> screenSize() const override;

    // AbstractRenderer interface
protected:
    bool update() override;

private:
    struct LensReciept
    {
        e172::Vector<double> point0;
        e172::Vector<double> point1;
        double coeficient;
    };

    struct Font
    {
        typedef std::map<int, TTF_Font *> sizes_t;
        std::string path;
        sizes_t sizes;
    };

private:
    Renderer(const std::string &title, const e172::Vector<std::uint32_t> &resolution);

    static void applyLensEffect(SDL_Surface *surface,
                                const e172::Vector<double> point0,
                                const e172::Vector<double> point1,
                                double coef);

private:
    static inline auto s_sdlInitialized = false;

    int64_t m_depth = 0;
    Camera *m_camera = nullptr;
    SDL_Window *m_window = nullptr;
    SDL_Surface *m_surface = nullptr;
    std::map<std::string, Font> m_fonts;
    bool m_lastFullscreen = false;
    e172::Vector<std::uint32_t> m_resolution;
    bool m_anaglyphEnabled = false;
    bool m_anaglyphEnabled2 = false;
    std::queue<LensReciept> m_lensQueue;
    e172::PriorityProcedure::Queue m_drawQueue;
};

} // namespace e172::impl::sdl
