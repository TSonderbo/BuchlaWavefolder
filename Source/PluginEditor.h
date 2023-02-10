/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ADSR.h"
#include "ScopeComponent.h"

//==============================================================================
/**
*/
class FMSynthesizerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FMSynthesizerAudioProcessorEditor (FMSynthesizerAudioProcessor&);
    ~FMSynthesizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    FMSynthesizerAudioProcessor& audioProcessor;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment);

    juce::Slider ampSlider;
    juce::Label ampLabel{ "Amplitude", "Amp" };
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> amplitudeAttachment;

    ADSR adsr;
    ScopeComponent scopeComponent;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSynthesizerAudioProcessorEditor)
};
