/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BuchlaAudioProcessorEditor::BuchlaAudioProcessorEditor (BuchlaAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
    , scopeComponent(audioProcessor.getAudioBufferQueue())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1060, 500);

    addAndMakeVisible(scopeComponent);
    setSliderWithLabel(ampSlider, ampLabel, audioProcessor.apvts, "amplitude", amplitudeAttachment);
}

BuchlaAudioProcessorEditor::~BuchlaAudioProcessorEditor()
{
}

//==============================================================================
void BuchlaAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void BuchlaAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    const auto paddingX = 5;
    const auto paddingY = 5;
    
    scopeComponent.setBounds(paddingX, paddingY, 1000, getHeight() - (2 * paddingY));
    ampSlider.setBounds(scopeComponent.getRight(), paddingY, 50, getHeight() - (2 * paddingY));
}

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void BuchlaAudioProcessorEditor::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment)
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