#ifndef METALANCER_PCMSOUND_HPP
#define METALANCER_PCMSOUND_HPP

#include "Sound.hpp"
#include <string>

namespace Metalancer
{

    class PCMData;

    class PCMSound : public Sound
    {


        PCMData* _pcmData;


        unsigned int _currentReadPosition;

        int Read(int sampleCount, float *data);

        int GetSamplingRateForNextRead();
        
        bool HasDataToRead();

    public:

        PCMSound();
        ~PCMSound();

        void SetData(PCMData* data);

    };

}

#endif
