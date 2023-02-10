/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMSynthesizerAudioProcessorEditor::FMSynthesizerAudioProcessorEditor (FMSynthesizerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
    , adsr("Amp Envelope", audioProcessor.apvts, "attack", "decay", "sustain", "release")
    , scopeComponent(audioProcessor.getAudioBufferQueue())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 800);

    addAndMakeVisible(adsr);
    addAndMakeVisible(scopeComponent);
    setSliderWithLabel(ampSlider, ampLabel, audioProcessor.apvts, "amplitude", amplitudeAttachment);
}

FMSynthesizerAudioProcessorEditor::~FMSynthesizerAudioProcessorEditor()
{
}

//==============================================================================
void FMSynthesizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void FMSynthesizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    const auto paddingX = 5;
    const auto paddingY = 35;
    const auto paddingY2 = 235;

    adsr.setBounds(paddingX, paddingY, 300, 200);
    ampSlider.setBounds(adsr.getRight(), paddingY, 50, 200);
    scopeComponent.setBounds(paddingX, 300, 1000, 500);

}

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void FMSynthesizerAudioProcessorEditor::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramId, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}