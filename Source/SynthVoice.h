/*
  ==============================================================================

	SynthVoice.h
	Created: 1 Dec 2022 11:05:25am
	Author:  Sonderbo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void updateAdsr(const float attack, const float decay, const float sustain, const float release);
	void updateFmAdsr(const float attack, const float decay, const float sustain, const float release);
private:

	juce::ADSR adsr;
	juce::ADSR::Parameters adsrParams;
	juce::AudioBuffer<float> synthBuffer;

	juce::dsp::Oscillator<float> osc{ [](float x) {return std::sin(x); } };
	juce::dsp::Oscillator<float> osc2{ [](float x) {return std::sin(x); } };
	juce::dsp::Gain<float> gain;
	bool isPrepared = false;

	//FM Envelope
	juce::ADSR fmAdsr;
	juce::ADSR::Parameters fmAdsrParams;
	
	//Scalars
	float modScalar{ 0.6f };
	float carrier2Scalar{ 1.0f };

	float fmDepth1{ 0.0f };
	float fmDepth2{ 0.25f };

	float scaleAmp{ 0.2f };
	float scaleDev{ 0.2f };


	juce::dsp::Oscillator<float> fmOsc{ [](float x) {return std::sin(x); } };
};