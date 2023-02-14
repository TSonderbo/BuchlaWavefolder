/*
  ==============================================================================

    BuchlaWavefolder.h
    Created: 24 Jan 2023 8:28:50am
    Author:  Sonderbo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FoldingBranch.h"

class BuchlaWavefolder
{
public:
    void prepareToPlay(double sampleRate);
    void setFundemental(float f0);
    float processSample(float vIn);
    void setAmplitude(float A);
private:
    float lowpass(float vIn);

    float sampleRate;

    //Folding branches
    FoldingBranch branch1{ 10000.0f, 100000.0f };
    FoldingBranch branch2{ 49900.0f, 43200.0f };
    FoldingBranch branch3{ 91000.0f, 56000.0f };
    FoldingBranch branch4{ 30000.0f, 68000.0f };
    FoldingBranch branch5{ 68000.0f, 33000.0f };

    //History
    float vIn_n1;
    float vOut_n1;

    //lowpass coeffs
    float b0;
    float b1;
    float a1;
};