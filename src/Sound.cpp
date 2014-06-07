#include "Sound.hpp"

#include <assert.h>
#include <iostream>

using namespace Metalancer;
using namespace std;

Sound::Sound() :
   _isPlaying(false),
   _currentTime(0),
   _timeProcessed(0)
{
}

Sound::~Sound()
{
}

void Sound::GetData(int samples, int streamSamplingRate, float *data, int& samplesFilled, int& returnedSamplingRate)
{
    lock_guard<mutex> lock(_mutex);

    float previousActionTime = _timeProcessed;

    bool applyAction = false;

    //Determine sample ammount before next action;
    int samplesToProcess = samples;


    if(_isPlaying && HasDataToRead())
    {

        returnedSamplingRate = GetSamplingRateForNextRead();

        if(_actionQueue.size() > 0)
        {
            float timespanToAction = _actionQueue.front().first - previousActionTime;
//            if(timespanToAction < 0)
//            {
//                timespanToAction = 0;
//            };
            int samplesToTimeSpan = timespanToAction * returnedSamplingRate;
            samplesToProcess = min(samplesToProcess, samplesToTimeSpan);
        }

        int samplesToRead = min((int)((float)samplesToProcess * ((float)returnedSamplingRate / (float)streamSamplingRate)), samplesToProcess);
        assert(samplesToRead > -1);
        samplesFilled = Read(samplesToRead, data);
    }
    else
    {
        returnedSamplingRate = streamSamplingRate;

        if(_actionQueue.size() > 0)
        {
            float timespanToAction = _actionQueue.front().first - previousActionTime;
            if(timespanToAction < 0)
            {
                timespanToAction = 0;
            }
            int samplesToTimeSpan = timespanToAction * returnedSamplingRate;
            samplesToProcess = min(samplesToProcess, samplesToTimeSpan);
        }

        samplesFilled = samplesToProcess;
        FillWithZero(samplesToProcess, data);
    }

    if(_actionQueue.size() > 0)
    {
        float timespanToAction = _actionQueue.front().first - previousActionTime;

        float timeProcessed = (float)samplesFilled / (float)returnedSamplingRate;

        if(timeProcessed >= timespanToAction)
        {
            applyAction = true;
        }
        else
        {
            applyAction = false;
        }
    }

    if(applyAction)
    {
        Action action = _actionQueue.front().second;
        switch(action)
        {
        case ActionPlay:
            _isPlaying = true;
            break;
        case ActionPause:
            _isPlaying = false;
            break;
        case ActionStop:
            _isPlaying = false;
            break;
        }

        _actionQueue.pop();
    }

//    samplesProcessed += samplesToProcess;
//    data += samplesToProcess * 2;
    
    if(returnedSamplingRate)

    _timeProcessed += ((float)samplesFilled / (float)returnedSamplingRate);
}

void Sound::Play()
{
    QueueAction(ActionPlay);
}

void Sound::Pause()
{
    QueueAction(ActionPause);
}

void Sound::Stop()
{
    QueueAction(ActionStop);
}

void Sound::Update(float timestep)
{
    _currentTime += timestep;
}

/*
 *
 * Private
 *
 */

void Sound::FillWithZero(int samples, float *data)
{
    for(int i = 0, count = samples * 2; i < count; ++i)
    {
       data[i] = 0.0f;
    }
}

void Sound::QueueAction(Action action)
{
    lock_guard<mutex> lock(_mutex);

    _actionQueue.push(make_pair(_currentTime, action));
}
