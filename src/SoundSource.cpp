#include "SoundSource.h"

#include "log.h"

SoundSource::SoundSource()
{
    alGenSources(1, &m_Source);
    alSourcef(m_Source, AL_PITCH, m_fPitch);
    alSourcef(m_Source, AL_GAIN, m_fGain);
    alSource3f(m_Source, AL_POSITION, m_Position[0], m_Position[1], m_Position[2]);
    alSource3f(m_Source, AL_VELOCITY, m_Velocity[0], m_Velocity[1], m_Velocity[2]);
    alSourcei(m_Source, AL_LOOPING, m_bLoop);
    alSourcei(m_Source, AL_BUFFER, m_Buffer);
}

SoundSource::~SoundSource()
{
    alDeleteSources(1, &m_Source);
}

void SoundSource::Play(const ALuint sound)
{
    if (sound != m_Buffer)
    {
        m_Buffer = sound;
        alSourcei(m_Source, AL_BUFFER, (ALint)m_Buffer);
    }

    alSourcePlay(m_Source);

    ALint state = AL_PLAYING;
    while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
        alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
    LOG("Sound has finished playing!");
}

