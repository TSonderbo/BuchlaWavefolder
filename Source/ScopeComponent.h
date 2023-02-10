/*
  ==============================================================================

    ScopeComponent.h
    Created: 9 Feb 2023 9:37:21am
    Author:  Sonderbo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioBufferQueue.h"

class ScopeComponent : public juce::Component,
    private juce::Timer
{
public:
    using Queue = AudioBufferQueue;

    //==============================================================================
    ScopeComponent(Queue& queueToUse);

    //==============================================================================
    void setFramesPerSecond(int framesPerSecond);

    //==============================================================================
    void paint(juce::Graphics& g) override;

    //==============================================================================
    void resized() override;

private:
    //==============================================================================
    Queue& audioBufferQueue;
    std::array<float, Queue::bufferSize> sampleData;

    juce::dsp::FFT fft{ Queue::order };
    using WindowFun = juce::dsp::WindowingFunction<float>;
    WindowFun windowFun{ (size_t)fft.getSize(), WindowFun::hann };
    std::array<float, 2 * Queue::bufferSize> spectrumData;

    //==============================================================================
    void timerCallback() override;

    //==============================================================================
    static void plot(const float* data,
        size_t numSamples,
        juce::Graphics& g,
        juce::Rectangle<float> rect,
        float scaler = float(1),
        float offset = float(0));
};