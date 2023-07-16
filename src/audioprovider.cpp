#include "audioprovider.h"

#include "private/initsdl.h"
#include <SDL2/SDL_mixer.h>
#include <e172/debug.h>

namespace e172::impl::sdl {

AudioProvider::AudioProvider()
{
    initSDL();
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
}

e172::AudioSample AudioProvider::loadAudioSample(const std::string &path)
{
    if (auto audio = Mix_LoadWAV(path.c_str())) {
        auto handle = new e172::AudioSample::Handle<Mix_Chunk *>(audio);
        return createAudioSample(handle, this, [](e172::AudioSample::DataPtr handle) {
            auto h = e172::AudioSample::castHandle<Mix_Chunk *>(handle);
            Mix_FreeChunk(h->c);
            delete h;
        });
    }
    return e172::AudioSample();
}

e172::AudioChannel AudioProvider::reserveChannel()
{
    m_currentChannelCount++;
    if (m_currentChannelCount > m_reservedChannelCount){
        m_reservedChannelCount = Mix_AllocateChannels(m_reservedChannelCount + ReserveStep);
        if (m_currentChannelCount > m_reservedChannelCount){
            e172::Debug::warning("Audio channel can not be created, created:",
                                 m_reservedChannelCount,
                                 "required:",
                                 m_currentChannelCount);
        }
    }

    int channel = 0;
    if (freeChannel.size() > 0){
        channel = freeChannel.front();
        freeChannel.pop();
    } else {
        channel = m_currentChannelCount - 1;
    }

    return createAudioChannel(
        new e172::AudioChannel::Handle<int>(channel),
        this,
        [this](e172::AudioChannel::DataPtr data) {
            auto c = e172::AudioChannel::castHandle<int>(data);
            Mix_Pause(c->c);
            freeChannel.push(c->c);
            delete c;
        },
        [this](e172::AudioChannel::DataPtr data, double volume) {
            auto c = e172::AudioChannel::castHandle<int>(data);
            Mix_Volume(c->c, static_cast<int>(MIX_MAX_VOLUME * volume * generalVolume()));
        },
        [](e172::AudioChannel::DataPtr data, const e172::AudioSample &sample, int loops) {
            const auto c = e172::AudioChannel::castHandle<int>(data);
            const auto s = audioSampleData<Mix_Chunk *>(sample);
            Mix_PlayChannelTimed(c->c, s, loops - 1, -1);
        },
        [](e172::AudioChannel::DataPtr data) {
            auto c = e172::AudioChannel::castHandle<int>(data);
            return Mix_Playing(c->c);
        },
        [](e172::AudioChannel::DataPtr data) {
            auto c = e172::AudioChannel::castHandle<int>(data);
            Mix_Pause(c->c);
        });
}

} // namespace e172::impl::sdl
