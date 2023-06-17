#pragma once

#include "sdlrenderer.h"
#include <src/graphics/abstractgraphicsprovider.h>

class SDLGraphicsProvider : public e172::AbstractGraphicsProvider {
public:
    SDLGraphicsProvider(const std::vector<std::string> &args,
                        const std::string &title,
                        const e172::Vector<std::uint32_t> &resolution);

    ~SDLGraphicsProvider() override;

    e172::Image imageFromSDLSurface(SDL_Surface *surface) const;

    // AbstractGraphicsProvider interface
public:
    e172::AbstractRenderer *renderer() const override;
    bool isValid() const override;
    e172::Image loadImage(const std::string &path) const override;
    e172::Image createImage(int width, int height) const override;
    e172::Image createImage(int width,
                            int height,
                            const ImageInitFunction &imageInitFunction) const override;
    e172::Image createImage(int width,
                            int height,
                            const ImageInitFunctionExt &imageInitFunction) const override;

    void loadFont(const std::string &name, const std::string &path) override;
    bool fontLoaded(const std::string &name) const override;

    // AbstractGraphicsProvider interface
protected:
    void destructImage(e172::SharedContainer::DataPtr ptr) const override;
    e172::SharedContainer::Ptr imageBitMap(e172::SharedContainer::DataPtr ptr) const override;

    e172::SharedContainer::DataPtr imageFragment(e172::SharedContainer::DataPtr ptr,
                                                 std::size_t x,
                                                 std::size_t y,
                                                 std::size_t &w,
                                                 std::size_t &h) const override;

    e172::SharedContainer::DataPtr transformImage(e172::SharedContainer::DataPtr ptr,
                                                  uint64_t) const override;

    bool saveImage(e172::SharedContainer::DataPtr ptr, const std::string &path) const override;

    e172::SharedContainer::DataPtr blitImages(e172::SharedContainer::DataPtr ptr0,
                                              e172::SharedContainer::DataPtr ptr1,
                                              int x,
                                              int y,
                                              std::size_t &w,
                                              std::size_t &h) const override;

private:
    SDLRenderer *m_renderer = nullptr;
};
