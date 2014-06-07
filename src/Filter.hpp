#ifndef METALANCER_FILTER_HPP
#define METALANCER_FILTER_HPP

namespace Metalancer
{
    class Filter
    {

        float _sampleRate;
        float _centerFrequency;
        float _bandwidthInOctaves;
        float _gainInDB;

        float _coefA1;
        float _coefA2;
        float _coefB0;
        float _coefB1;
        float _coefB2;

        float _previousInput1;
        float _previousInput2;
        float _previousOutput1;
        float _previousOutput2;

        float ProcessSample(float sample);

    public:

        Filter();

        void Reset();

        void Setup(float samplingRate, float centerFrequency, float bandwidthInOctaves, float gain);

        void Process(int sampleCount, int stride, float* samples);


    };



}

#endif
