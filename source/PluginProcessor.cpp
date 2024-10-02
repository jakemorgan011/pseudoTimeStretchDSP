/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PseudotimestretchAudioProcessor::PseudotimestretchAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    _constructValueTreeState();
    time = ValueTreeState->getRawParameterValue("time");
    dryWet = ValueTreeState->getRawParameterValue("dryWet");
}

PseudotimestretchAudioProcessor::~PseudotimestretchAudioProcessor()
{
}

//==============================================================================
const juce::String PseudotimestretchAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PseudotimestretchAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PseudotimestretchAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PseudotimestretchAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PseudotimestretchAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PseudotimestretchAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PseudotimestretchAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PseudotimestretchAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PseudotimestretchAudioProcessor::getProgramName (int index)
{
    return {};
}

void PseudotimestretchAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PseudotimestretchAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    pseudoTS.prepareToPlay(sampleRate);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PseudotimestretchAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PseudotimestretchAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PseudotimestretchAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // correct this. it needs splits and repeats variables.
    pseudoTS.setParameters(*time, *dryWet, 0, 0);
    
    pseudoTS.processBlock(buffer);
}

//==============================================================================
bool PseudotimestretchAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PseudotimestretchAudioProcessor::createEditor()
{
    return new PseudotimestretchAudioProcessorEditor (*this);
}

//==============================================================================
void PseudotimestretchAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = ValueTreeState->copyState();
    std::unique_ptr<juce::XmlElement>xml(state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PseudotimestretchAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    ValueTreeState->replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PseudotimestretchAudioProcessor();
}

void PseudotimestretchAudioProcessor::_constructValueTreeState(){
    ValueTreeState.reset(new juce::AudioProcessorValueTreeState(*this, nullptr, juce::Identifier("pseudoTS"), {
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("time", 1), "time",juce::NormalisableRange<float>(0.1f,1.0f,0.1f),0.5f),
        
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("dryWet", 1), "dryWet",juce::NormalisableRange<float>(0.1f,1.0f,0.1f),0.5f),
        
        
    }));
}
