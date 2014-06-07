#include "PCMSound.hpp"

using namespace Metalancer;
using namespace std;

#include <fstream>
#include <limits.h>
#include <assert.h>
#include <algorithm>

PCMSound::PCMSound() :
    _pcmData(nullptr),
    _isOpen(false),
    _currentReadPosition(0)
{

}

PCMSound::~PCMSound()
{


}

bool PCMSound::OpenFile(const string &name)
{
    _isOpen = false;
    
    // Open the file
    ifstream stream(name.c_str(), ios::binary);
    if(!stream.is_open())
    {
        return false;
    }

    // Read the header.
    stream.read((char*)&_header, sizeof(_header));

    // Allocate memory for the pcm data
    if(_pcmData != nullptr)
    {
        delete [] _pcmData;
    }

    _pcmData = new char[_header.subchunk2Size];

    // Read the pcm data
    stream.read(_pcmData, _header.subchunk2Size);

    // All done.
    stream.close();
    
    _currentReadPosition = 0;
    
    _isOpen = true;
    return true;
}


/*
 *
 * Private
 *
 */

int PCMSound::Read(int samples, float *data)
{
    // TODO: Nullcheck pcm data. Null pcm data means pcm data not opened.

    char* pcmData = _pcmData + _currentReadPosition;

    int sampleCount = min((unsigned int)samples * 2, _header.subchunk2Size - _currentReadPosition);
    
    assert(_currentReadPosition < _header.subchunk2Size);

    for(int i = 0; i < sampleCount; i += 2)
    {
        if(_header.bitsPerSample == 8)
        {
            data[i] = (float)*pcmData / 255.0f;
            data[i + 1] = (float)*(pcmData+1) / 255.0f;
        }
        else
        {
            data[i] = (float)(*(short*)pcmData) / SHRT_MAX;
            data[i + 1] = (float)(*(short*)pcmData + 1) / SHRT_MAX;
        }

        pcmData += (_header.bitsPerSample / 8) * 2;

    }
    
    // Fill remaining data with zero if we still need more samples
    for(int i = 0; i < samples - sampleCount; ++i)
    {
        data[sampleCount + i] = 0.0f;
    }

    _currentReadPosition += (_header.bitsPerSample / 8) * sampleCount;

    return samples;
}

int PCMSound::GetSamplingRateForNextRead()
{
    if(!_isOpen)
    {
        return 0;
    }
    return _header.samplingRate;
}

bool PCMSound::HasDataToRead()
{
    return _isOpen && _currentReadPosition < _header.subchunk2Size;
}
