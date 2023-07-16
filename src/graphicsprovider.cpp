#include "graphicsprovider.h"

#include "private/initsdl.h"
#include "private/spm.h"
#include <SDL2/SDL_image.h>

namespace e172::impl::sdl {

GraphicsProvider::GraphicsProvider()
{
    initSDL();
}

std::shared_ptr<AbstractRenderer> GraphicsProvider::createRenderer(
    const std::string &title, const e172::Vector<uint32_t> &resolution) const
{
    const auto renderer = std::make_shared<Renderer>(Renderer::Private{}, title, resolution);
    installParentToRenderer(*renderer);
    return renderer;
}

e172::Image GraphicsProvider::imageFromSDLSurface(SDL_Surface *surface) const
{
    if(surface) {
        return imageFromData(new e172::Image::Handle<SDL_Surface *>(surface),
                             surface->w,
                             surface->h);
    }
    return e172::Image();
}

e172::Image GraphicsProvider::loadImage(const std::string &path) const
{
    return imageFromSDLSurface(IMG_Load(path.c_str()));
}

e172::Image GraphicsProvider::createImage(std::size_t width, std::size_t height) const
{
    assert(width <= std::numeric_limits<int>::max());
    assert(height <= std::numeric_limits<int>::max());
    return imageFromSDLSurface(SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000));
}

e172::Image GraphicsProvider::createImage(std::size_t width,
    std::size_t height,
    const e172::AbstractGraphicsProvider::ImageInitFunction &imageInitFunction) const
{
    assert(width <= std::numeric_limits<int>::max());
    assert(height <= std::numeric_limits<int>::max());
    SDL_Surface *surface
        = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    if(surface) {
        imageInitFunction(reinterpret_cast<uint32_t*>(surface->pixels));
        return imageFromSDLSurface(surface);
    }
    return e172::Image();
}

e172::Image GraphicsProvider::createImage(std::size_t width,
    std::size_t height,
    const e172::AbstractGraphicsProvider::ImageInitFunctionExt &imageInitFunction) const
{
    assert(width <= std::numeric_limits<int>::max());
    assert(height <= std::numeric_limits<int>::max());
    SDL_Surface *surface
        = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    if(surface) {
        imageInitFunction(width, height, reinterpret_cast<uint32_t*>(surface->pixels));
        return imageFromSDLSurface(surface);
    }
    return e172::Image();
}

void GraphicsProvider::loadFont(const std::string &name, const std::filesystem::__cxx11::path &path)
{
    m_fonts[name] = Font{path, std::map<int, TTF_Font *>()};
}

bool GraphicsProvider::fontLoaded(const std::string &name) const
{
    return m_fonts.find(name) != m_fonts.end();
}

e172::Vector<uint32_t> GraphicsProvider::screenSize() const
{
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    assert(displayMode.w >= 0);
    assert(displayMode.h >= 0);
    return e172::Vector<std::uint32_t>(displayMode.w, displayMode.h);
}

void GraphicsProvider::destructImage(e172::SharedContainer::DataPtr ptr) const
{
    const auto handle = e172::Image::castHandle<SDL_Surface *>(ptr);
    SDL_FreeSurface(handle->c);
    delete handle;
}

e172::SharedContainer::Ptr GraphicsProvider::imageBitMap(e172::SharedContainer::DataPtr ptr) const
{
    return e172::Image::castHandle<SDL_Surface *>(ptr)->c->pixels;
}

e172::SharedContainer::DataPtr GraphicsProvider::imageFragment(e172::SharedContainer::DataPtr ptr,
                                                               std::size_t x,
                                                               std::size_t y,
                                                               std::size_t &w,
                                                               std::size_t &h) const
{
    const auto handle = e172::Image::castHandle<SDL_Surface *>(ptr);
    const auto newHandle = new e172::Image::Handle<SDL_Surface *>(
        spm::cutOutSurface(handle->c, x, y, w, h));
    w = newHandle->c->w;
    h = newHandle->c->h;
    return newHandle;
}

e172::SharedContainer::DataPtr GraphicsProvider::transformImage(e172::SharedContainer::DataPtr ptr,
                                                                uint64_t) const
{
    return ptr;
}

bool GraphicsProvider::saveImage(e172::SharedContainer::DataPtr ptr, const std::string &path) const
{
    return 0 == IMG_SavePNG(e172::Image::castHandle<SDL_Surface *>(ptr)->c, path.c_str());
}

e172::SharedContainer::DataPtr GraphicsProvider::blitImages(e172::SharedContainer::DataPtr ptr0,
                                                            e172::SharedContainer::DataPtr ptr1,
                                                            std::ptrdiff_t x,
                                                            std::ptrdiff_t y,
                                                            std::size_t &w,
                                                            std::size_t &h) const
{
    const auto handle0 = e172::Image::castHandle<SDL_Surface *>(ptr0);
    const auto handle1 = e172::Image::castHandle<SDL_Surface *>(ptr1);

    const auto result = spm::copySurface(handle0->c);
    if(!result)
        return nullptr;

    SDL_Rect resultRect;
    resultRect.x = x;
    resultRect.y = y;
    SDL_BlitSurface(handle1->c, nullptr, result, &resultRect);

    assert(result->w >= 0);
    assert(result->h >= 0);
    w = result->w;
    h = result->h;

    return new e172::Image::Handle<SDL_Surface *>(result);
}

} // namespace e172::impl::sdl
