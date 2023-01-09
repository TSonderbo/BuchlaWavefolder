/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMSynthesizerAudioProcessor::FMSynthesizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

FMSynthesizerAudioProcessor::~FMSynthesizerAudioProcessor()
{

}

//==============================================================================
const juce::String FMSynthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FMSynthesizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FMSynthesizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FMSynthesizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FMSynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FMSynthesizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FMSynthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FMSynthesizerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FMSynthesizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void FMSynthesizerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FMSynthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void FMSynthesizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FMSynthesizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FMSynthesizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            auto& attack = *apvts.getRawParameterValue("attack");
            auto& decay = *apvts.getRawParameterValue("decay");
            auto& sustain = *apvts.getRawParameterValue("sustain");
            auto& release = *apvts.getRawParameterValue("release");

            voice->updateAdsr(attack, decay, sustain, release);
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool FMSynthesizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FMSynthesizerAudioProcessor::createEditor()
{
    return new FMSynthesizerAudioProcessorEditor (*this);
}

//==============================================================================
void FMSynthesizerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FMSynthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FMSynthesizerAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout FMSynthesizerAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //Inserts params here...
    
    //Combo box
    auto choices = juce::StringArray{ "Sine", "Other choice"};
    params.push_back(std::make_unique<juce::AudioParameterChoice>("sound_type", "Sound type", choices, 0));
    
    //ADSR
	params.push_back(std::make_unique<juce::AudioParameterFloat>("attack", "Attack", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("decay", "Decay", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("sustain", "Sustain", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.9f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("release", "Release", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.4f));

    //Chowning params
    params.push_back(std::make_unique<juce::AudioParameterFloat>("carrier1", "Carrier frequency 1", juce::NormalisableRange<float> {20.0f, 20000.0f}, 300.0f)); //P5
    params.push_back(std::make_unique<juce::AudioParameterFloat>("carrier2", "Carrier frequency 2", juce::NormalisableRange<float> {20.0f, 20000.0f}, 2100.0f)); //P12
    params.push_back(std::make_unique<juce::AudioParameterFloat>("modulating", "Modulating frequency", juce::NormalisableRange<float> {0.1f, 1.0f}, 300.0f)); //P6
    params.push_back(std::make_unique<juce::AudioParameterFloat>("modidx1", "Modulating index 1", juce::NormalisableRange<float> {0.1f, 25.0f}, 1.0f)); //P7
    params.push_back(std::make_unique<juce::AudioParameterFloat>("modidx2", "Modulating index 2", juce::NormalisableRange<float> {0.1f, 25.0f}, 3.0f)); //P8
    params.push_back(std::make_unique<juce::AudioParameterFloat>("scaleamp", "Scale Amp", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.2f)); //P10
    params.push_back(std::make_unique<juce::AudioParameterFloat>("scaledev", "Scale Dev", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.5f)); //P11

    return { params.begin(), params.end() };
}