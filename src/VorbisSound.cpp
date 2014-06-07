#include "VorbisSound.hpp"

#include <iostream>

#include <limits.h>

using namespace Metalancer;
using namespace std;

VorbisSound::VorbisSound()
{
    _vorbisReadBuffer = new char[4096];
    _previousBitstream = -1;
}

VorbisSound::~VorbisSound()
{
    delete [] _vorbisReadBuffer;
}

bool VorbisSound::OpenFile(const string &fileName)
{
    int result = ov_fopen(fileName.c_str(), &_vorbisFile);
    if(result != 0)
    {
        return false;
    }

    vorbis_info* info = ov_info(&_vorbisFile, -1);

    cout << "Channels " << info->channels << endl;
    cout << "Sampling Rate " << info->rate << endl;
    cout << "Bitrate Upper " << info->bitrate_upper << endl;
    cout << "Bitrate Lower " << info->bitrate_lower << endl;
    cout << "Bitrate Nominal: " << info->bitrate_nominal << endl;

    return true;
}

/*
 *
 * Private
 *
 */

int VorbisSound::Read(int samples, float *data)
{
    int currentBitstream = 0;
    int length = samples * sizeof(short) * 2;
    int totalBytesRead = 0;
    while(totalBytesRead < length)
    {
        int readSize = min(length - totalBytesRead, 4096);
        int bytesRead = ov_read(&_vorbisFile, _vorbisReadBuffer, readSize, 0, 2, 1, &currentBitstream);

        if(_previousBitstream != currentBitstream)
        {
            vorbis_info* info = ov_info(&_vorbisFile, currentBitstream);

            cout << "NEW BITSTREAM:::::::::::::::::" << endl;
            cout << "Channels " << info->channels << endl;
            cout << "Sampling Rate " << info->rate << endl;
            cout << "Bitrate Upper " << info->bitrate_upper << endl;
            cout << "Bitrate Lower " << info->bitrate_lower << endl;
            cout << "Bitrate Nominal: " << info->bitrate_nominal << endl;

            _previousBitstream = currentBitstream;
        }



        if(bytesRead < 0)
        {
            cerr << "ERROR!" << endl;
            break;
        }

        if(bytesRead == 0)
        {
            // eof
            break;
        }

        int sampleCount = bytesRead / sizeof(short);
        int totalSamplesRead = totalBytesRead / sizeof(short);
        for(int sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++)
        {
            int outIndex = sampleIndex + (totalSamplesRead);
            data[outIndex] = (float)(((short*)_vorbisReadBuffer)[sampleIndex]) / ((float)SHRT_MAX);
        }

        totalBytesRead += bytesRead;
    }



    if(totalBytesRead < length)
    {
        //Fill the rest of the buffer with zero.
        for(int sampleIndex = totalBytesRead / sizeof(short);
            sampleIndex < length / sizeof(short); ++sampleIndex)
        {
            data[sampleIndex] = 0.0f;
        }
    }

    return samples;
}



