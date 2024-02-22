#ifndef MUSIC_BUFFER_H
#define MUSIC_BUFFER_H

#include <AL/al.h>
#include <sndfile.h>

class MusicBuffer
{
public:
    void Play();
    void Stop();
    void Pause();
    void Resume();

    void SetCurrentPlayPosition(const int nPercent);

    void UpdateBufferStream();
    
    bool IsPlaying() const;
    int GetElapsedPercent() const;

    MusicBuffer(const char* sFilename);
    ~MusicBuffer(); 

private:
    static const int BUFFER_SAMPLES = 8192;
    static const int NUM_BUFFERS = 4;

    ALuint m_Source;
    ALuint m_Buffers[NUM_BUFFERS];
    SNDFILE* m_File;
    SF_INFO m_SFInfo;
    short* m_MemBuffer;
    ALenum m_Format;   

    int m_nSlens = 0;
    int m_nElapsedPercent = 0;

    MusicBuffer() = delete;

};

#endif // MUSIC_BUFFER_H
