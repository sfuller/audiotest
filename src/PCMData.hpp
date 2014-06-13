#ifndef METALANCER_PCMDATA_HPP
#define METALANCER_PCMDATA_HPP

#include <string>
#include <mutex>

namespace Metalancer
{

    class PCMData
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

        std::mutex _mutex;

        char* _pcmData;

        bool _isOpen;

    public:

        PCMData();

        bool OpenFile(const std::string& filename);

        int CopyData(unsigned int sampleIndex, unsigned int sampleCount, int channels, float *dest);

        int GetSamplingRate() const;

        int SampleCount() const;

        bool IsOpen() const;
    };

}

#endif
