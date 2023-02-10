/*
  ==============================================================================

    AudioBufferQueue.h
    Created: 9 Feb 2023 9:37:02am
    Author:  Sonderbo

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

class AudioBufferQueue
{
public:
    //==============================================================================
    static constexpr size_t order = 9;
    static constexpr size_t bufferSize = 1U << order;
    static constexpr size_t numBuffers = 5;

    //==============================================================================
    void push(const float* dataToPush, size_t numSamples);

    //==============================================================================
    void pop(float* outputBuffer);

private:

    //==============================================================================
    juce::AbstractFifo abstractFifo{ numBuffers };
    std::array<std::array<float, bufferSize>, numBuffers> buffers;
};