#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include <AL/al.h>

class SoundSource
{
public:
    SoundSource();
    ~SoundSource();
    
    void Play(const ALuint sound);

private:
    ALuint m_Source;
    float m_fPitch = 1.0f;
    float m_fGain = 1.0f;
    float m_Position[3] = { 0.0f, 0.0f, 0.0f };
    float m_Velocity[3] = { 0.0f, 0.0f, 0.0f };
    bool m_bLoop = false;
    ALuint m_Buffer = 0;

};

#endif // SOUND_SOURCE_H
