#pragma once

#include <src/abstracteventprovider.h>

union SDL_Event;

class SDLEventProvider : public e172::AbstractEventProvider
{
public:
    SDLEventProvider() = default;

    // AbstractEventHandler interface
public:
    std::optional<e172::Event> pullEvent() override;
};
