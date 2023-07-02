#pragma once

#include <e172/math/vector.h>

struct SDL_Surface;

class VisualEffect {
public:
    VisualEffect() = default;
    VisualEffect(e172::Vector<std::int32_t> offset);
    virtual SDL_Surface *operator()(SDL_Surface *input);
    void setOffset(const e172::Vector<std::int32_t> &value);
    virtual ~VisualEffect() = default;
    const e172::Vector<std::int32_t> &offset() const { return m_offset; }

private:
    e172::Vector<std::int32_t> m_offset;
};
