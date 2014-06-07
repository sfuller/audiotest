#include "Filter.hpp"
#include <math.h>

#define PI 3.14159265358979f

using namespace Metalancer;

Filter::Filter()
    : _previousInput1(0),
      _previousInput2(0),
      _previousOutput1(0),
      _previousOutput2(0)
{
}


void Filter::Setup(float samplingRate, float centerFrequency, float bandwidthInOctaves, float gain)
{
    float a = pow(10, gain / 40);
    float w0 = 2 * PI * (centerFrequency / samplingRate);
    float c = cos(w0);
    float s = sin(w0);
    float alpha = s * sinh( (log(2) / 2) * bandwidthInOctaves * (w0 / s) ); //log is ln, right?

    // Coefficients
    _coefB0 = 1 + alpha * a;
    _coefB1 = -2 * c;
    _coefB2 = 1- alpha * a;
    float coefA0 = 1 + alpha / a;
    _coefA1 = -2 * c;
    _coefA2 = 1 - alpha / a;

    _coefB0 /= coefA0;
    _coefB1 /= coefA0;
    _coefB2 /= coefA0;
    _coefA1 /= coefA0;
    _coefA2 /= coefA0;
    // don't bother making coefA0 1 because it isn't used anywhere in our biquad transfer function

}

void Filter::Reset()
{
    _previousInput1 = 0;
    _previousInput2 = 0;
    _previousOutput1 = 0;
    _previousOutput1 = 0;
}

void Filter::Process(int sampleCount, int stride, float *samples)
{
    for(int i = 0; i < sampleCount; ++i)
    {
        *samples = ProcessSample(*samples);
        samples += stride;
    }
}

/*
 *
 * Private
 *
 */

float Filter::ProcessSample(float sample)
{
    float output = _coefB0 * sample + _coefB1 * _previousInput1 + _coefB2 * _previousInput2 -
        _coefA1 * _previousOutput1 - _coefA2 * _previousOutput2;

    _previousInput2 = _previousInput1;
    _previousInput1 = sample;
    _previousOutput2 = _previousOutput1;
    _previousOutput1 = output;

    return output;
}
