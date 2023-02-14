/*
  ==============================================================================

    ScopeDataCollector.cpp
    Created: 9 Feb 2023 9:36:16am
    Author:  Sonderbo

  ==============================================================================
*/

#include "ScopeDataCollector.h"

ScopeDataCollector::ScopeDataCollector(AudioBufferQueue& queueToUse)
    : audioBufferQueue(queueToUse)
{

}

void ScopeDataCollector::process(const float* data, size_t numSamples)
{
    size_t index = 0;

    if (state == State::waitingForTrigger)
    {
        while (index++ < numSamples)
        {
            auto currentSample = *data++;

            if (abs(currentSample) >= triggerLevel) // && prevSample < triggerLevel
            {
                numCollected = 0;
                state = State::collecting;
                break;
            }

            prevSample = currentSample;
        }
    }

    if (state == State::collecting)
    {
        while (index++ < numSamples)
        {
            buffer[numCollected++] = *data++;

            if (numCollected == buffer.size())
            {
                audioBufferQueue.push(buffer.data(), buffer.size());
                state = State::waitingForTrigger;
                prevSample = float(100);
                break;
            }
        }
    }
}