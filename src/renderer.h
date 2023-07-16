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

class GraphicsProvider;

class Renderer : public e172::AbstractRenderer
{
    friend GraphicsProvider;
    struct Private
    {};

public:
    static const int DefaultFontSize;

    Renderer(Private, const std::string &title, const e172::Vector<std::uint32_t> &resolution);
    ~Renderer();

    // AbstractRenderer interface
public:
    void fill(Color color) override;
    void drawPixel(const e172::Vector<double> &point, Color color) override;
    void drawLine(const e172::Vector<double> &point0,
                  const e172::Vector<double> &point1,
                  Color color) override;

    void drawRect(const e172::Vector<double> &point0,
                  const e172::Vector<double> &point1,
                  Color color,
                  const e172::ShapeFormat &format = e172::ShapeFormat(false)) override;

    void drawSquare(const e172::Vector<double> &point, double radius, Color color) override;
    void drawCircle(const e172::Vector<double> &point, double radius, Color color) override;
    void drawDiagonalGrid(const e172::Vector<double> &point1,
                          const e172::Vector<double> &point2,
                          double interval,
                          Color color) override;

    void drawImage(const e172::Image &image,
                   const e172::Vector<double> &center,
                   double angle,
                   double zoom) override;

    e172::Vector<double> drawString(const std::string &string,
                                    const e172::Vector<double> &pos,
                                    Color color,
                                    const e172::TextFormat &format = e172::TextFormat()) override;

    void modifyBitmap(const std::function<void(e172::Color *bitmap)> &modifier) override;

    void enableEffect(uint64_t effect) override;
    void disableEffect(uint64_t effect) override;

    e172::Vector<std::uint32_t> resolution() const override { return m_resolution; }
    void setResolution(const e172::Vector<std::uint32_t> &value) override;

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

private:
    static void applyLensEffect(SDL_Surface *surface,
                                const e172::Vector<double> point0,
                                const e172::Vector<double> point1,
                                double coef);

private:
    int64_t m_depth = 0;
    Camera *m_camera = nullptr;
    SDL_Window *m_window = nullptr;
    SDL_Surface *m_surface = nullptr;
    bool m_lastFullscreen = false;
    e172::Vector<std::uint32_t> m_resolution;
    bool m_anaglyphEnabled = false;
    bool m_anaglyphEnabled2 = false;
    std::queue<LensReciept> m_lensQueue;
    e172::PriorityProcedure::Queue m_drawQueue;
};

} // namespace e172::impl::sdl
