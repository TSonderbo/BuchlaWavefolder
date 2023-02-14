/*
  ==============================================================================

    FoldingBranch.h
    Created: 5 Feb 2023 11:31:52am
    Author:  Sonderbo

  ==============================================================================
*/


#pragma once

#include <math.h>
#include <JuceHeader.h>

class FoldingBranch
{
public:
    FoldingBranch(float rk_1, float rk_3);

    void prepareToPlay(double sampleRate);
    void setFundemental(float f0);
    float processSample(float vIn);
    void setAmplitude(float A);
private:
    // Constants
    const float vs = 6.0f; //Voltage constant
    const double pi = 3.14159265358979323846;

    // Branch specific
    float rk_1; //Resistor
    const float rk_2 = 100000.0f; //Resistor
    float rk_3; //Resistor
    float clip_thresh; //Clipping threshold

    //Util
    double sampleRate = 48000.0f; //Sampling rate
    float f0 = 100.0f; //Fundemental frequency
    float A = 1.0f; //Amplitude
    float T = 1.0f /48000.0f; //Sampling period

    //State
    float period_ts = 0.0f; //Current point in the period
    float delta = 0.0f; //Normalized frequency
    float clipArr[4] { 0, 0, 0, 0 }; //Array for storing clipping points
    float vk_n1 = 0.0f; //
    bool flagArr[4]{0, 0, 0, 0};
};