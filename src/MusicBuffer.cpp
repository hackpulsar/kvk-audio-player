#include "MusicBuffer.h"

#include <iostream>
#include <AL/alext.h>
#include <QDebug>

void MusicBuffer::Play()
{
    ALsizei i;

    alGetError(); // clear any al errors

    alSourceRewind(m_Source);
    alSourcei(m_Source, AL_BUFFER, 0); // basically clears the buffer

    // Fill the buffer queue
    for (i = 0; i < NUM_BUFFERS; i++)
    {
        // Get data from file and fill the buffer
        sf_count_t slen = sf_readf_short(m_File, m_MemBuffer, BUFFER_SAMPLES);

        if (slen < 1) break;

        slen *= m_SFInfo.channels * (sf_count_t)sizeof(short);
        alBufferData(m_Buffers[i], m_Format, m_MemBuffer, (ALsizei)slen, m_SFInfo.samplerate);
    }

    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Error buffering for playback" << std::endl;
        return;
    }

    alSourceQueueBuffers(m_Source, i, m_Buffers);
    alSourcePlay(m_Source);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Error starting playback" << std::endl;
        return;
    }
}

void MusicBuffer::Pause() { alSourcePause(m_Source); }
void MusicBuffer::Stop() { alSourceStop(m_Source); }
void MusicBuffer::Resume() { alSourcePlay(m_Source); }

void MusicBuffer::SetCurrentPlayPosition(const int nPercent)
{
    sf_count_t frames = (sf_count_t)((nPercent * m_SFInfo.frames) / 100);
    sf_seek(m_File, frames, SEEK_SET);
    m_nSlens = frames / BUFFER_SAMPLES;
    m_nElapsedPercent = nPercent;
}

void MusicBuffer::UpdateBufferStream()
{
    ALint processed, state;

    alGetError(); // clear error 

    // Get relevant source info
    alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
    alGetSourcei(m_Source, AL_BUFFERS_PROCESSED, &processed);
    if (alGetError() != AL_NO_ERROR)
    {
        std::cerr << "Error checking music source state" << std::endl;
        return;
    }

    while (processed > 0)
    {
        ALuint bufid;
        sf_count_t slen;

        alSourceUnqueueBuffers(m_Source, 1, &bufid);
        processed--;

        // Read the next chunk of data, refill the buffer and queue it
        // back on the source
        slen = sf_readf_short(m_File, m_MemBuffer, BUFFER_SAMPLES);
        ++m_nSlens;
        if (slen > 0)
        {
            slen *= m_SFInfo.channels * (sf_count_t)sizeof(short);
            alBufferData(bufid, m_Format, m_MemBuffer, (ALsizei)slen, m_SFInfo.samplerate);
            alSourceQueueBuffers(m_Source, 1, &bufid);
        }

        m_nElapsedPercent = (int)(((BUFFER_SAMPLES * m_nSlens) * 100) / m_SFInfo.frames);

        if (alGetError() != AL_NO_ERROR)
        {
            std::cerr << "Error buffering music data" << std::endl;
            return;
        }
    }

    // Make sure the source has not underrun
    if (state != AL_PLAYING && state != AL_PAUSED)
    {
        ALint queued;

        alGetSourcei(m_Source, AL_BUFFERS_QUEUED, &queued);
        if (queued == 0)
            return;

        alSourcePlay(m_Source);
        if (alGetError() != AL_NO_ERROR)
        {
            std::cerr << "Error restarting music playback" << std::endl;
            return;
        }
    }
}

bool MusicBuffer::IsPlaying() const
{
    ALint state;

    alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

int MusicBuffer::GetElapsedPercent() const { return m_nElapsedPercent; }

MusicBuffer::MusicBuffer(const char* sFilename)
{
    alGenSources(1, &m_Source);
    alGenBuffers(NUM_BUFFERS, m_Buffers);

    size_t nFrameSize;
    m_File = sf_open(sFilename, SFM_READ, &m_SFInfo);
    if (!m_File)
    {
        std::cerr << "Error: Could not open file " << sFilename << std::endl;
        return;
    }

    // Get the sound format and figure out the OpenAL format
    if (m_SFInfo.channels == 1)
        m_Format = AL_FORMAT_MONO16;
    else if (m_SFInfo.channels == 2)
        m_Format = AL_FORMAT_STEREO16;
    else if (m_SFInfo.channels == 3)
    {
        if (sf_command(m_File, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            m_Format = AL_FORMAT_BFORMAT2D_16;
    }
    else if (m_SFInfo.channels == 4)
    {
        if (sf_command(m_File, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            m_Format = AL_FORMAT_BFORMAT3D_16;
    }

    if (!m_Format)
    {
        sf_close(m_File);
        m_File = nullptr;
        std::cerr << "Unsupported channel count from file" << std::endl;
        return;
    }

    nFrameSize = ((size_t)BUFFER_SAMPLES * (size_t)m_SFInfo.channels) * sizeof(short);
    m_MemBuffer = static_cast<short*>(malloc(nFrameSize));

    qDebug() << nFrameSize << " " << m_SFInfo.frames;
}

MusicBuffer::~MusicBuffer()
{
    alDeleteSources(1, &m_Source);
    if (m_File)
        sf_close(m_File);
    m_File = nullptr;
    free(m_MemBuffer);
    alDeleteBuffers(NUM_BUFFERS, m_Buffers);
}

