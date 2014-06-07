#ifndef METALANCER_SOUND_HPP
#define METALANCER_SOUND_HPP

#include <mutex>
#include <queue>

namespace Metalancer
{

    class Sound
    {

        enum Action
        {
            ActionPlay,
            ActionPause,
            ActionStop
        };

        std::mutex _mutex;

        bool _isPlaying;

        float _currentTime;

        float _timeProcessed;

        std::queue<std::pair<float, Action> > _actionQueue;

        virtual int Read(int sampleCount, float* data) = 0;

        virtual int GetSamplingRateForNextRead() = 0;
        
        virtual bool HasDataToRead() = 0;

        void FillWithZero(int samples, float* data);

        void QueueAction(Action action);

    public:

        Sound();
        virtual ~Sound();

        void GetData(int samples, int streamSamplingRate, float* data, int& samplesUsed, int& returnedSamplingRate);

        virtual void Stop();

        virtual void Pause();

        virtual void Play();

        virtual void Update(float timestep);

    };


}

#endif
