#pragma once

#include "visualeffect.h"
#include <math.h>

class Anaglyph : public VisualEffect {
public:
    Anaglyph() = default;
    Anaglyph(e172::Vector<int32_t> offset)
        : VisualEffect(offset)
    {}

    // VisualEffect interface
public:
    SDL_Surface *operator()(SDL_Surface *input);
};
