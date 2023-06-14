#include "sdlgraphicsprovider.h"
#include "spm.h"

#include "SDL2/SDL_image.h"

SDLGraphicsProvider::SDLGraphicsProvider(const std::vector<std::string> &args,
                                         const std::string &title,
                                         const e172::Vector<uint32_t> &resolution)
    : e172::AbstractGraphicsProvider(args)
{
    m_renderer = new SDLRenderer(title, resolution);
    installParentToRenderer(m_renderer);
}

SDLGraphicsProvider::~SDLGraphicsProvider() {
    delete m_renderer;
}

e172::Image SDLGraphicsProvider::imageFromSDLSurface(SDL_Surface *surface) const {
    if(surface) {
        return imageFromData(new e172::Image::handle<SDL_Surface*>(surface), surface->w, surface->h);
    }
    return e172::Image();
}

e172::AbstractRenderer *SDLGraphicsProvider::renderer() const {
    return m_renderer;
}

bool SDLGraphicsProvider::isValid() const {
    return true;
}

e172::Image SDLGraphicsProvider::loadImage(const std::string &path) const {
    return imageFromSDLSurface(IMG_Load(path.c_str()));
}

e172::Image SDLGraphicsProvider::createImage(int width, int height) const {
    return imageFromSDLSurface(SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000));
}

e172::Image SDLGraphicsProvider::createImage(int width, int height, const e172::AbstractGraphicsProvider::ImageInitFunction &imageInitFunction) const {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    if(surface) {
        imageInitFunction(reinterpret_cast<uint32_t*>(surface->pixels));
        return imageFromSDLSurface(surface);
    }
    return e172::Image();
}

e172::Image SDLGraphicsProvider::createImage(int width, int height, const e172::AbstractGraphicsProvider::ImageInitFunctionExt &imageInitFunction) const {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    if(surface) {
        imageInitFunction(width, height, reinterpret_cast<uint32_t*>(surface->pixels));
        return imageFromSDLSurface(surface);
    }
    return e172::Image();
}

void SDLGraphicsProvider::loadFont(const std::string &name, const std::string &path) {
    m_renderer->m_fonts[name] = SDLRenderer::Font { path, std::map<int, TTF_Font*>() };
}

bool SDLGraphicsProvider::fontLoaded(const std::string &name) const {
    return m_renderer->m_fonts.find(name) != m_renderer->m_fonts.end();
}

void SDLGraphicsProvider::destructImage(e172::SharedContainer::data_ptr ptr) const {
    const auto handle = e172::Image::handle_cast<SDL_Surface*>(ptr);
    SDL_FreeSurface(handle->c);
    delete handle;
}

e172::SharedContainer::ptr SDLGraphicsProvider::imageBitMap(e172::SharedContainer::data_ptr ptr) const {
    return e172::Image::handle_cast<SDL_Surface*>(ptr)->c->pixels;
}

e172::SharedContainer::data_ptr SDLGraphicsProvider::imageFragment(e172::SharedContainer::data_ptr ptr, std::size_t x, std::size_t y, std::size_t &w, std::size_t &h) const {
    const auto handle = e172::Image::handle_cast<SDL_Surface*>(ptr);
    const auto newHandle = new e172::Image::handle<SDL_Surface*>(SPM::CutOutSurface(handle->c, x, y, w, h));
    w = newHandle->c->w;
    h = newHandle->c->h;
    return newHandle;
}

e172::SharedContainer::data_ptr SDLGraphicsProvider::transformImage(e172::SharedContainer::data_ptr ptr, uint64_t) const {
    return ptr;
}

bool SDLGraphicsProvider::saveImage(e172::SharedContainer::data_ptr ptr, const std::string & path) const {
    return 0 == IMG_SavePNG(e172::Image::handle_cast<SDL_Surface*>(ptr)->c, path.c_str());
}

e172::SharedContainer::data_ptr SDLGraphicsProvider::blitImages(e172::SharedContainer::data_ptr ptr0, e172::SharedContainer::data_ptr ptr1, int x, int y, std::size_t &w, std::size_t &h) const {
    const auto handle0 = e172::Image::handle_cast<SDL_Surface*>(ptr0);
    const auto handle1 = e172::Image::handle_cast<SDL_Surface*>(ptr1);

    const auto result = SPM::CopySurface(handle0->c);
    if(!result)
        return nullptr;

    SDL_Rect resultRect;
    resultRect.x = x;
    resultRect.y = y;
    SDL_BlitSurface(handle1->c, nullptr, result, &resultRect);

    w = result->w;
    h = result->h;

    return new e172::Image::handle<SDL_Surface*>(result);
}
