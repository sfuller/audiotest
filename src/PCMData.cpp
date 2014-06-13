#include "PCMData.hpp"
#include <fstream>
#include <assert.h>

using namespace Metalancer;
using namespace std;

PCMData::PCMData()
    : _pcmData(nullptr),
      _isOpen(false)
{
}

bool PCMData::OpenFile(const string &name)
{
    lock_guard<mutex> lock(_mutex);

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

    //_currentReadPosition = 0;

    _isOpen = true;
    return true;

}

int PCMData::CopyData(unsigned int sampleIndex, unsigned int sampleCount, int channels, float* dest)
{
    lock_guard<mutex> lock(_mutex);

    char* pcmData = _pcmData + sampleIndex * ((_header.bitsPerSample / 8) * 2); //TODO: Replace 2 with channels in pcm data.

    int before = sampleCount;

    sampleCount = min(sampleCount, (_header.subchunk2Size / ((_header.bitsPerSample / 8) * 2)) - sampleIndex );

    assert(sampleIndex * ((_header.bitsPerSample / 8) * 2) < _header.subchunk2Size);

    for(int i = 0; i < sampleCount; i++)
    {
        if(_header.bitsPerSample == 8)
        {
            dest[0] = (float)*pcmData / 255.0f;
            dest[1] = (float)*(pcmData+1) / 255.0f;
        }
        else
        {
            dest[0] = (float)(*(short*)pcmData) / SHRT_MAX;
            dest[1] = (float)(*(short*)pcmData + 1) / SHRT_MAX;
        }

        dest += 2; //Replace with requested channels

        pcmData += (_header.bitsPerSample / 8) * 2;
    }

    return sampleCount;
}

int PCMData::GetSamplingRate() const
{
    if(!_isOpen)
    {
        return 0;
    }
    return _header.samplingRate;
}

int PCMData::SampleCount() const
{
    if(!_isOpen)
    {
        return 0;
    }
    return _header.subchunk2Size / ((_header.bitsPerSample / 8) * 2);
}

bool PCMData::IsOpen() const
{
    return _isOpen;
}
