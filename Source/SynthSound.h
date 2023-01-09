/*
  ==============================================================================

    SynthSound.h
    Created: 1 Dec 2022 11:05:53am
    Author:  Sonderbo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};