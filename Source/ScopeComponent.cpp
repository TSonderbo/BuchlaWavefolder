/*
  ==============================================================================

    ScopeComponent.cpp
    Created: 9 Feb 2023 9:37:21am
    Author:  Sonderbo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ScopeComponent.h"

//==============================================================================

ScopeComponent::ScopeComponent(Queue& queueToUse) : audioBufferQueue(queueToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    sampleData.fill(float(0));
    setFramesPerSecond(30);
}

void ScopeComponent::setFramesPerSecond(int framesPerSecond)
{
    jassert(framesPerSecond > 0 && framesPerSecond < 1000);
    startTimerHz(framesPerSecond);
}


void ScopeComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);

    auto area = getLocalBounds();
    auto h = (float)area.getHeight();
    auto w = (float)area.getWidth();

    // Oscilloscope
    auto scopeRect = juce::Rectangle<float>{ float(0), float(0), w, h };
    plot(sampleData.data(), sampleData.size(), g, scopeRect, 0.5f, h/2);
}

void ScopeComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void ScopeComponent::timerCallback()
{
    audioBufferQueue.pop(sampleData.data());
    juce::FloatVectorOperations::copy(spectrumData.data(), sampleData.data(), (int)sampleData.size());

    auto fftSize = (size_t)fft.getSize();

    jassert(spectrumData.size() == 2 * fftSize);
    windowFun.multiplyWithWindowingTable(spectrumData.data(), fftSize);
    fft.performFrequencyOnlyForwardTransform(spectrumData.data());

    static constexpr auto mindB = float(-160);
    static constexpr auto maxdB = float(0);

    for (auto& s : spectrumData)
        s = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(s) - juce::Decibels::gainToDecibels(float(fftSize))), mindB, maxdB, float(0), float(1));

    repaint();
}

void ScopeComponent::plot(const float* data,
    size_t numSamples,
    juce::Graphics& g,
    juce::Rectangle<float> rect,
    float scaler,
    float offset)
{
    auto w = rect.getWidth();
    auto h = rect.getHeight();
    auto right = rect.getRight();

    auto center = rect.getBottom() - offset;
    auto gain = h * scaler;

    for (size_t i = 1; i < numSamples; ++i)
        g.drawLine({ juce::jmap(float(i - 1), float(0), float(numSamples - 1), float(right - w), float(right)),
                      center - gain * data[i - 1],
                      juce::jmap(float(i), float(0), float(numSamples - 1), float(right - w), float(right)),
                      center - gain * data[i] });
}