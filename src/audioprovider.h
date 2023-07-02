#pragma once

#include <e172/audio/abstractaudioprovider.h>
#include <queue>

namespace e172::impl::sdl {

class AudioProvider : public e172::AbstractAudioProvider
{
    enum { ReserveStep = 10 };

    int m_reservedChannelCount = 0;
    int m_currentChannelCount = 0;
    std::queue<int> freeChannel;
public:
    AudioProvider();

    // AbstractAudioProvider interface
public:
    virtual e172::AudioSample loadAudioSample(const std::string &path) override;
    virtual e172::AudioChannel reserveChannel() override;
};

} // namespace e172::impl::sdl
