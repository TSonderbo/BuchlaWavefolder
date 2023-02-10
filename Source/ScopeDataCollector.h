/*
  ==============================================================================

    ScopeDataCollector.h
    Created: 9 Feb 2023 9:36:16am
    Author:  Sonderbo

  ==============================================================================
*/

#include "AudioBufferQueue.h"

#pragma once
class ScopeDataCollector
{
public:
    //==============================================================================
    ScopeDataCollector(AudioBufferQueue& queueToUse);

    //==============================================================================
    void process(const float* data, size_t numSamples);

private:
    //==============================================================================
    AudioBufferQueue& audioBufferQueue;
    std::array<float, AudioBufferQueue::bufferSize> buffer;
    size_t numCollected;
    float prevSample = float(100);

    static constexpr auto triggerLevel = float(0.05);

    enum class State { waitingForTrigger, collecting } state{ State::waitingForTrigger };
};