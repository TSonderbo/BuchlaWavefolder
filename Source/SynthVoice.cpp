/*
  ==============================================================================

	SynthVoice.cpp
	Created: 1 Dec 2022 11:05:25am
	Author:  Sonderbo

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	//Return true if sound is valid
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	adsr.noteOn();
	osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));

	float modfreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * modScalar;
	fmOsc.setFrequency(modfreq);

	float carrier2Freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * carrier2Scalar;
	osc2.setFrequency(carrier2Freq);
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	adsr.noteOff();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = numChannels;

	osc.prepare(spec);
	gain.prepare(spec);

	gain.setGainLinear(0.3f);

	adsr.setSampleRate(sampleRate);

	isPrepared = true;
}

void SynthVoice::updateAdsr(const float attack, const float decay, const float sustain, const float release)
{
	adsrParams.attack = attack;
	adsrParams.decay = decay;
	adsrParams.sustain = sustain;
	adsrParams.release = release;

	adsr.setParameters(adsrParams);
}

void SynthVoice::updateFmAdsr(const float attack, const float decay, const float sustain, const float release)
{
	fmAdsrParams.attack = attack;
	fmAdsrParams.decay = decay;
	fmAdsrParams.sustain = sustain;
	fmAdsrParams.release = release;

	fmAdsr.setParameters(fmAdsrParams);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (!isVoiceActive()) return;

	while (--numSamples >= 0)
	{

		//TODO - NOT This basically
		//Calculate the three oscilators used

		float fmFreq = fmOsc.processSample(0);

		// P7*P6
		float fmMod = fmFreq * fmDepth1;

		// Addition
		float mod = fmMod + 1; // fmAdsr.getNextSample();
		
		mod = fmFreq * mod;

		float modC = mod + osc.processSample(0);

		float amp = adsr.getNextSample();

		modC *= amp;

		mod *= scaleDev;

		mod += osc2.processSample(0);

		mod *= scaleAmp;

		float sample = mod + modC;
		
		sample = gain.processSample(sample);
		for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
		{
			outputBuffer.addSample(channel, startSample, sample);
		}
		startSample++;
	}

	if (!adsr.isActive())
		clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}