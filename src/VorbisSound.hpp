#ifndef METALANCER_VORBISSOUND_HPP
#define METALANCER_VORBISSOUND_HPP

#include "Sound.hpp"
#include <string>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <queue>
#include <mutex>

namespace Metalancer
{


    class VorbisSound : public Sound
    {

        OggVorbis_File _vorbisFile;

        char* _vorbisReadBuffer;

        int _previousBitstream;

        //std::mutex _mutex;

        int Read(int samples, float* data);


    public:

        VorbisSound();
        ~VorbisSound();


        bool OpenFile(const std::string& fileName);

        //void GetData(int length, float* data);
    };


}


#endif
