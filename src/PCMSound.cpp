#include "PCMSound.hpp"

#include <fstream>
#include <limits.h>
#include <assert.h>
#include <algorithm>

#include <iostream>

#include "PCMData.hpp"

using namespace Metalancer;
using namespace std;

PCMSound::PCMSound() :
    _pcmData(nullptr),
    _currentReadPosition(0)
{

}

PCMSound::~PCMSound()
{
}

void PCMSound::SetData(PCMData *data)
{
    _pcmData = data;
    _currentReadPosition = 0;
}

/*
 *
 * Private
 *
 */

int PCMSound::Read(int samples, float *data)
{

    if(_pcmData == nullptr)
    {
        return 0;
    }

    int samplesRead = _pcmData->CopyData(_currentReadPosition, samples, 2, data);

    _currentReadPosition += samplesRead;

    samplesRead *= 2;
    
    // Fill remaining data with zero if we still need more samples
    for(int i = 0; i < (samples * 2) - samplesRead; ++i)
    {
        data[samplesRead + i] = 0.0f;
    }
    if((samples * 2) - samplesRead > 0)
    {
        cout << (samples * 2) - samplesRead << endl;
    }

    return samples;
}

int PCMSound::GetSamplingRateForNextRead()
{
    if(_pcmData == nullptr)
    {
        return 0;
    }
    return _pcmData->GetSamplingRate();
}

bool PCMSound::HasDataToRead()
{
    if(_pcmData == nullptr)
    {
        return false;
    }
    return _pcmData->IsOpen() && _currentReadPosition < _pcmData->SampleCount();
}
