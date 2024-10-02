/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PseudotimestretchAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PseudotimestretchAudioProcessorEditor (PseudotimestretchAudioProcessor&);
    ~PseudotimestretchAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::Slider timeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> timeSliderAttachment;
    
    juce::Slider dryWetSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetSliderAttachment;
    
    void _updateTempoSync();
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PseudotimestretchAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PseudotimestretchAudioProcessorEditor)
};
