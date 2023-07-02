#pragma once

#include <e172/abstracteventprovider.h>

union SDL_Event;

namespace e172::impl::sdl {

class EventProvider : public e172::AbstractEventProvider
{
public:
    EventProvider() = default;

    // AbstractEventHandler interface
public:
    std::optional<e172::Event> pullEvent() override;
};

} // namespace e172::impl::sdl
