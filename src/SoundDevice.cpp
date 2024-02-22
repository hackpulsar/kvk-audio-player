#include "SoundDevice.h"

#include <AL/al.h>
#include <stdio.h>

static SoundDevice* s_pInstance = nullptr;

SoundDevice* SoundDevice::get()
{
    if (s_pInstance == nullptr) init();
    return s_pInstance;
}

void SoundDevice::init()
{
    if (s_pInstance == nullptr)
        s_pInstance = new SoundDevice();
}

SoundDevice::SoundDevice()
{
    m_pALCDevice = alcOpenDevice(nullptr); // nullptr -> get default device
    if (!m_pALCDevice)
        throw("Failed to get sound device");

    m_pALCContext = alcCreateContext(m_pALCDevice, nullptr);
    if(!m_pALCContext)
        throw("Failed to create sound context");

    if(!alcMakeContextCurrent(m_pALCContext))
        throw("Failed to make sound context current");

    const ALCchar* name = nullptr;
    if (alcIsExtensionPresent(m_pALCDevice, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(m_pALCDevice, ALC_ALL_DEVICES_SPECIFIER);
    if (!name || alcGetError(m_pALCDevice) != ALC_NO_ERROR)
        name = alcGetString(m_pALCDevice, ALC_DEVICE_SPECIFIER);
    printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_pALCContext);
    alcCloseDevice(m_pALCDevice);
}

