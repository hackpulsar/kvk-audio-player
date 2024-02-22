#ifndef SOUND_DEVICE_H
#define SOUND_DEVICE_H

#include <AL/alc.h>

class SoundDevice
{
public:
    static SoundDevice* get();
    static void init();

private:
    SoundDevice();
    ~SoundDevice();

    ALCdevice* m_pALCDevice;
    ALCcontext* m_pALCContext;
};

#endif // SOUND_DEVICE_H
