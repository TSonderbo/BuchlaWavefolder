/*
  ==============================================================================

    BuchlaWavefolder.cpp
    Created: 24 Jan 2023 8:28:50am
    Author:  Sonderbo

  ==============================================================================
*/

#include "BuchlaWavefolder.h"

void BuchlaWavefolder::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate;
    this->T = 1 / sampleRate;

    branch1.prepareToPlay(sampleRate);
    branch2.prepareToPlay(sampleRate);
    branch3.prepareToPlay(sampleRate);
    branch4.prepareToPlay(sampleRate);
    branch5.prepareToPlay(sampleRate);
}

void BuchlaWavefolder::setFundemental(float f0)
{
    branch1.setFundemental(f0);
    branch2.setFundemental(f0);
    branch3.setFundemental(f0);
    branch4.setFundemental(f0);
    branch5.setFundemental(f0);
}

void BuchlaWavefolder::setAmplitude(float A)
{
    branch1.setAmplitude(A);
    branch2.setAmplitude(A);
    branch3.setAmplitude(A);
    branch4.setAmplitude(A);
    branch5.setAmplitude(A);
}

float BuchlaWavefolder::processSample(float vIn)
{
    

    float v_1 = branch1.processSample(vIn) * 12.000f;
    float v_2 = branch2.processSample(vIn) * 27.777f;
    float v_3 = branch3.processSample(vIn) * 21.428f;
    float v_4 = branch4.processSample(vIn) * 17.647f;
    float v_5 = branch5.processSample(vIn) * 36.363f;
    float v_d = vIn * 5.0f;

    float v_out = ((v_1 + v_2 + v_3) + ((v_4 + v_5 + v_d) * -1)) * -1;

    //lowpass
    v_out = lowpass(v_out);

    if (vIn > 1)
    {
        DBG("Vout larger than 1");
    }

    return v_out;
}

float BuchlaWavefolder::lowpass(float vIn)
{
    const double pi = 3.14159265358979323846;
    const float fc = 1 / (2 * pi * 1200000 * 1.0f * powf(10.0f, -10.0f));
    const float wc = 2 * pi * fc;

    float b0 = (wc * T) / (2 + wc * T);
    float b1 = b0;
   
    float a1 = (wc * T - 2) / (wc * T + 2);

    float out = b0 * vIn + b1 * vIn_n1 + a1 * vOut_n1;

    vOut_n1 = out;
    vIn_n1 = vIn;

    return out;
}