#ifndef METALANCER_PCMSOUND_HPP
#define METALANCER_PCMSOUND_HPP

#include "Sound.hpp"
#include <string>

namespace Metalancer
{

    class PCMSound : public Sound
    {

        struct WaveHeader
        {
            // RIFF Chunk
            char chunkId[4]; // "RIFF"
            int chunkSize;
            char format[4]; // "WAVE"
            // fmt sub-chunk
            char subchunk1Id[4]; // "fmt "
            int subchunk1Size;
            short audioFormat;
            short numChannels;
            int samplingRate;
            int byteRate;
            short blockAlign;
            short bitsPerSample;
            //data sub-chunk
            char subcunk2Id[4]; // "data"
            int subchunk2Size;
        } _header;

        char* _pcmData;
        
        bool _isOpen;

        unsigned int _currentReadPosition;

        int Read(int sampleCount, float *data);

        int GetSamplingRateForNextRead();
        
        bool HasDataToRead();

    public:

        PCMSound();
        ~PCMSound();

        bool OpenFile(const std::string& name);

    };

}

#endif
