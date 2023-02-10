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
    const float vs = 6.0f;
    const float thresh = 0.01f;
    const double pi = 3.14159265358979323846;

    // Branch specific
    float rk_1;
    const float rk_2 = 100000.0f;
    float rk_3;
    float clip_thresh;

    //Util
    double sampleRate = 48000;
    float f0 = 100;
    float A = 1;
    float T = 1/48000;
    float ph = 0;
    float delta = 0;

    float clipArr[4] { 0, 0, 0, 0 };

    // History
    float vk_n1 = 0;
    bool flagArr[4]{0, 0, 0, 0};
};