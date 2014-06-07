#include "AudioSystem.hpp"

#include <portaudio.h>
#include <iostream>
#include <math.h>
#include <assert.h>

#include "Sound.hpp"
#include "Filter.hpp"

using namespace Metalancer;
using namespace std;

AudioSystem::UserData::UserData(AudioSystem* system)
    : currentFrame(0),
      system(system),
      _tempBuffer(new float[8192])
{
}

AudioSystem::AudioSystem()
    : _userData(this),
      _stream(nullptr)
{

    int error = Pa_Initialize();
    if(error != paNoError)
    {
        cerr << Pa_GetErrorText(error) << endl;
        return;
    }

    // Open stream
    error = Pa_OpenDefaultStream(&_stream, 0, 2, paFloat32, 44100, 1024, &AudioSystem::StreamCallback, &_userData );
    if(error != paNoError)
    {
        cerr << Pa_GetErrorText(error) << endl;
        return;
    }
}

void AudioSystem::Play()
{
    Pa_Sleep(1000);

    int error = Pa_StartStream(_stream);
    if(error != paNoError)
    {
        cerr << Pa_GetErrorText(error);
        return;
    }
}

AudioSystem::~AudioSystem()
{
    int error = Pa_Terminate();
    if(error != paNoError)
    {
        cerr << Pa_GetErrorText(error) << endl;
    }
}

void AudioSystem::AddSound(Sound *sound)
{
    _sounds.push_back(sound);
}

/*
 *
 * Private
 *
 */

int AudioSystem::StreamCallback(const void *inputBuffer,
                                void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData)
{
    UserData* data = (UserData*)userData;
    float* out = (float*)outputBuffer;

    int soundCount = data->system->_sounds.size();

    for(int sampleIndex = 0; sampleIndex < framesPerBuffer * 2; ++sampleIndex)
    {
        out[sampleIndex] = 0.0f;
    }

    int samplingRate = 44100;

    for(int i = 0; i < soundCount; ++i)
    {
        int totalSamplesFilled = 0;
        while(totalSamplesFilled < framesPerBuffer)
        {
            int samplesFilled = 0;
            int returnedSamplingRate;
            data->system->_sounds.at(i)->GetData(framesPerBuffer, samplingRate, data->_tempBuffer, samplesFilled, returnedSamplingRate);
            if(returnedSamplingRate == 0)
            {
                continue;
            }
            if(returnedSamplingRate != samplingRate)
            {
                // Needs resampling
                int newSamplesCount = samplesFilled * (samplingRate / returnedSamplingRate);

                assert(newSamplesCount <= framesPerBuffer);

                float sampleStride = samplingRate / returnedSamplingRate;

                for(int sampleIndex = 0, count = samplesFilled; sampleIndex < count; ++sampleIndex)
                {
                    int newIndex = sampleIndex * sampleStride;

                    //int tempi = (int)(i * ((float)returnedSamplingRate / (float)samplingRate));
                    out[(totalSamplesFilled * 2) + (newIndex * 2)] = data->_tempBuffer[sampleIndex * 2];
                    out[(totalSamplesFilled * 2) + (newIndex * 2) + 1] = data->_tempBuffer[sampleIndex * 2 + 1];
                }

                // Filter!
                Filter filter;
                float cornerFrequency = returnedSamplingRate / 2.0f;
                //float q = cornerFrequency / cornerFrequency;
                float q = 0.7071f;
                float bandwidthInOctaves = (2.0f / log(2.0)) * asinh(1.0f / (2.0f * q));
                filter.Setup(samplingRate, cornerFrequency , bandwidthInOctaves, 0);
                filter.Process(newSamplesCount, 2, out + (totalSamplesFilled * 2));
                filter.Reset();
                filter.Process(newSamplesCount, 2, out + (totalSamplesFilled * 2) + 1);


                totalSamplesFilled += newSamplesCount;
            }
            else
            {
                // No resampling needed.

                for(int i = 0; i < samplesFilled * 2; ++i)
                {
                    out[(totalSamplesFilled * 2) + i] += data->_tempBuffer[i];
                }

                totalSamplesFilled += samplesFilled;
            }
        }
    }

    //for your health
    for(unsigned int i = 0; i < framesPerBuffer * 2; ++i )
    {
        out[i] = min(1.0f, max(-1.0f, out[i]));
    }

    return 0;
}
