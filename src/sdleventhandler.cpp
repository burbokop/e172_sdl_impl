#include "sdleventhandler.h"

#include <SDL2/SDL.h>

SDLEventHandler::SDLEventHandler() {
    for(size_t i = 0; i < bufferSize; i++) {
        m_singlePressedKeys[i] = false;
        m_holdedKeys[i] = false;
    }
}

void SDLEventHandler::handleEvent(SDL_Event event) {
    if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.scancode < bufferSize) {
            m_holdedKeys[event.key.keysym.scancode] = true;
            m_singlePressedKeys[event.key.keysym.scancode] = true;

            if (event.key.keysym.scancode != SDL_SCANCODE_LSHIFT) {
                if (m_holdedKeys[SDL_SCANCODE_LSHIFT]) {
                    m_textBuffer.append(1, toUpperKeySym(event.key.keysym.sym));
                } else {
                    m_textBuffer.append(1, event.key.keysym.sym);
                }
            }
        }
    } else if(event.type == SDL_KEYUP) {
        if(event.key.keysym.scancode < bufferSize) {
            m_holdedKeys[event.key.keysym.scancode] = false;
        }
    } else if(event.type == SDL_MOUSEMOTION) {
        m_mousePosition = e172::Vector(event.motion.x, event.motion.y);
    } else if (event.type == SDL_QUIT) {
        m_exitFlag = true;
    }
}

char SDLEventHandler::toUpperKeySym(char c) {
    if(c == ',') {
        return '<';
    } else if(c == '.') {
        return '>';
    } else {
        return toupper(c);
    }
}


bool SDLEventHandler::exitFlag() const {
    if(m_keyboardEnabled) {
        return m_exitFlag;
    } else {
        return false;
    }
}

bool SDLEventHandler::keyHolded(e172::Scancode key) const {
    if(m_keyboardEnabled && key < bufferSize)
        return m_holdedKeys[key];

    return false;
}

bool SDLEventHandler::keySinglePressed(e172::Scancode key) const {
    if(m_keyboardEnabled && key < bufferSize)
        return m_singlePressedKeys[key];

    return false;
}

void SDLEventHandler::update() {
    for(size_t i = 0; i < bufferSize; i++) {
        m_singlePressedKeys[i] = false;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        handleEvent(event);
    }
}

e172::Vector SDLEventHandler::mousePosition() const {
    return m_mousePosition;
}


std::string SDLEventHandler::pullText() {
    if(m_keyboardEnabled) {
        auto result = m_textBuffer;
        m_textBuffer.clear();
        return result;
    } else {
        return std::string();
    }
}

void SDLEventHandler::enableKeyboard() {
    m_keyboardEnabled = true;
}

void SDLEventHandler::disableKeyboard() {
    m_keyboardEnabled = false;
}
