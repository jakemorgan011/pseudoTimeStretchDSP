/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PseudotimestretchAudioProcessorEditor::PseudotimestretchAudioProcessorEditor (PseudotimestretchAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    timeSlider.setRange(0.1f,1.0f);
    timeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    timeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    addAndMakeVisible(timeSlider);
    timeSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),"time",timeSlider));
    
    dryWetSlider.setRange(0.1f,1.0f);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    dryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    addAndMakeVisible(dryWetSlider);
    dryWetSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),"dryWet",timeSlider));
    setSize (400, 300);
}

PseudotimestretchAudioProcessorEditor::~PseudotimestretchAudioProcessorEditor()
{
}

//==============================================================================
void PseudotimestretchAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::whitesmoke);
}

void PseudotimestretchAudioProcessorEditor::resized()
{
    timeSlider.setBounds(0,0,150,150);
    dryWetSlider.setBounds(200,0,150,150);
}
