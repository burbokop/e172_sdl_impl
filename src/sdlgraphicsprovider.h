#ifndef SDLGRAPHICSPROVIDER_H
#define SDLGRAPHICSPROVIDER_H

#include "sdlrenderer.h"

#include <src/graphics/abstractgraphicsprovider.h>




class SDLGraphicsProvider : public e172::AbstractGraphicsProvider {
    SDLRenderer *m_renderer = nullptr;

public:
    SDLGraphicsProvider(const std::vector<std::string> &args, const char *title, int x, int y);
    ~SDLGraphicsProvider() override;

    e172::Image imageFromSDLSurface(SDL_Surface *surface) const;

    // AbstractGraphicsProvider interface
public:
    virtual e172::AbstractRenderer *renderer() const override;
    virtual bool isValid() const override;
    virtual e172::Image loadImage(const std::string &path) const override;
    virtual e172::Image createImage(int width, int height) const override;
    virtual e172::Image createImage(int width, int height, const ImageInitFunction &imageInitFunction) const override;
    virtual e172::Image createImage(int width, int height, const ImageInitFunctionExt &imageInitFunction) const override;

    virtual void loadFont(const std::string &name, const std::string &path) override;
    virtual bool fontLoaded(const std::string &name) const override;

protected:
    virtual void destructImage(e172::SharedContainer::data_ptr ptr) const override;
    virtual e172::SharedContainer::ptr imageBitMap(e172::SharedContainer::data_ptr ptr) const override;
    virtual e172::SharedContainer::data_ptr imageFragment(e172::SharedContainer::data_ptr ptr, int x, int y, int &w, int &h) const override;
    virtual e172::SharedContainer::data_ptr transformImage(e172::SharedContainer::data_ptr ptr, uint64_t) const override;
    virtual bool saveImage(e172::SharedContainer::data_ptr ptr, const std::string &path) const override;    
    virtual e172::SharedContainer::data_ptr blitImages(e172::SharedContainer::data_ptr ptr0, e172::SharedContainer::data_ptr ptr1, int x, int y, int &w, int &h) const override;

};



#endif // SDLGRAPHICSPROVIDER_H
