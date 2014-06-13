#ifndef METALANCER_AUDIOSYSTEM_HPP
#define METALANCER_AUDIOSYSTEM_HPP

#include <portaudio.h>
#include <unordered_set>

#include <mutex>

namespace Metalancer
{
    class Sound;

    class AudioSystem
    {

        class UserData
        {
        public:

            UserData(AudioSystem* system);

            unsigned int currentFrame;
            AudioSystem* system;

            float* _tempBuffer;
        };

        std::mutex _mutex;

        UserData _userData;

        PaStream* _stream;

        std::unordered_set<Sound*> _sounds;

        static int StreamCallback(const void* inputBuffer,
                                  void* outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo* timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void* userData
                                  );




    public:

        AudioSystem();
        ~AudioSystem();

        void AddSound(Sound* sound);

        void RemoveSound(Sound* sound);

        void Play();

    };

}


#endif
