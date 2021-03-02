#ifndef TOON_AUDIO_DRIVER_HPP
#define TOON_AUDIO_DRIVER_HPP

#include <Toon/Config.hpp>

namespace Toon {

class TOON_ENGINE_API AudioDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(AudioDriver)

    AudioDriver() = default;

    virtual ~AudioDriver() = defualt;

}; // class AudioDriver

void SetAudioDriver(AudioDriver * driver);

AudioDriver * GetAudioDriver();

} // namespace Toon

#endif // TOON_AUDIO_DRIVER_HPP