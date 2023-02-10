/*
  ==============================================================================

    AudioBufferQueue.cpp
    Created: 9 Feb 2023 9:37:02am
    Author:  Sonderbo

  ==============================================================================
*/

#include "AudioBufferQueue.h"

void AudioBufferQueue::push(const float* dataToPush, size_t numSamples)
{
    jassert(numSamples <= bufferSize);

    int start1, size1, start2, size2;
    abstractFifo.prepareToWrite(1, start1, size1, start2, size2);

    jassert(size1 <= 1);
    jassert(size2 == 0);

    if (size1 > 0)
        juce::FloatVectorOperations::copy(buffers[(size_t)start1].data(), dataToPush, (int)juce::jmin(bufferSize, numSamples));

    abstractFifo.finishedWrite(size1);
}

void AudioBufferQueue::pop(float* outputBuffer)
{
    int start1, size1, start2, size2;
    abstractFifo.prepareToRead(1, start1, size1, start2, size2);

    jassert(size1 <= 1);
    jassert(size2 == 0);

    if (size1 > 0)
        juce::FloatVectorOperations::copy(outputBuffer, buffers[(size_t)start1].data(), (int)bufferSize);

    abstractFifo.finishedRead(size1);
}