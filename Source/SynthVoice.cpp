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
	
	double freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

	osc.setFrequency(freq);
	buchla.setFundemental(freq);
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
	buchla.prepareToPlay(sampleRate);
	gain.setGainLinear(0.1f);
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

void SynthVoice::updateAmplitude(const float A)
{
	buchla.setAmplitude(A);
	this->A = A;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (!isVoiceActive()) return;

	while (--numSamples >= 0)
	{
		float sample = osc.processSample(0) * A;

		sample = buchla.processSample(sample);

		//sample *= adsr.getNextSample();

		//sample = gain.processSample(sample);

		
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