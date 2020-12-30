#include "sdlgraphicsprovider.h"
#include "spm.h"

#include "SDL2/SDL_image.h"

SDLGraphicsProvider::SDLGraphicsProvider(const std::vector<std::string> &args, const char *title, int x, int y) : e172::AbstractGraphicsProvider(args) {
    m_renderer = new SDLRenderer(title, x, y);
    installParentToRenderer(m_renderer);
}

SDLGraphicsProvider::~SDLGraphicsProvider() {
    delete m_renderer;
}

e172::Image SDLGraphicsProvider::__createSDLImage(SDL_Surface *surface) const {
    if(surface) {
        return __createImage(new e172::Image::handle<SDL_Surface*>(surface), this, surface->w, surface->h, [](e172::Image::data_ptr d) {
            const auto handle = e172::Image::handle_cast<SDL_Surface*>(d);
            SDL_FreeSurface(handle->c);
            delete handle;
        }, [](e172::Image::data_ptr ptr) -> e172::Image::ptr {
            return e172::Image::handle_cast<SDL_Surface*>(ptr)->c->pixels;
        }, [](e172::Image::data_ptr d, int x, int y, int &w, int &h) -> e172::Image::data_ptr {
            const auto handle = e172::Image::handle_cast<SDL_Surface*>(d);
            const auto newHandle = new e172::Image::handle<SDL_Surface*>(SPM::CutOutSurface(handle->c, x, y, w, h));
            w = newHandle->c->w;
            h = newHandle->c->h;
            return newHandle;
        });
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
    return __createSDLImage(IMG_Load(path.c_str()));
}

e172::Image SDLGraphicsProvider::createImage(int width, int height) const {
    return __createSDLImage(SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000));
}

e172::Image SDLGraphicsProvider::createImage(int width, int height, const e172::AbstractGraphicsProvider::ImageInitFunction &imageInitFunction) const {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    if(surface) {
        imageInitFunction(reinterpret_cast<uint32_t*>(surface->pixels));
        return __createSDLImage(surface);
    }
    return e172::Image();
}

e172::Image SDLGraphicsProvider::createImage(int width, int height, const e172::AbstractGraphicsProvider::ImageInitFunctionExt &imageInitFunction) const {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    if(surface) {
        imageInitFunction(width, height, reinterpret_cast<uint32_t*>(surface->pixels));
        return __createSDLImage(surface);
    }
    return e172::Image();
}

void SDLGraphicsProvider::loadFont(const std::string &name, const std::string &path) {
    m_renderer->m_fonts[name] = SDLRenderer::Font { path, std::map<int, TTF_Font*>() };
}
