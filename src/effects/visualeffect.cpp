#include "visualeffect.h"

VisualEffect::VisualEffect(e172::Vector<int32_t> offset)
    : m_offset(offset)
{}

SDL_Surface *VisualEffect::operator()(SDL_Surface *input) {
    return input;
}

void VisualEffect::setOffset(const e172::Vector<int32_t> &value)
{
    m_offset = value;
}
