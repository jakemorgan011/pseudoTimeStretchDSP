/*
  ==============================================================================

    psuedoTS.cpp
    Created: 3 Sep 2024 12:40:32pm
    Author:  Jake Morgan

  ==============================================================================
*/

#include "psuedoTS.h"

pseudoTS::pseudoTS(){
    smoothedTime.setCurrentAndTargetValue(0.5f);
    smoothedDryWet.setCurrentAndTargetValue(0.5f);
}

pseudoTS::~pseudoTS(){
}

void pseudoTS::prepareToPlay(float inSampleRate){
    sampleRate = inSampleRate;
    circularBuffer.setSize(2, inSampleRate * 4);
    
    smoothedTime.reset(inSampleRate, 0.001f);
    smoothedDryWet.reset(inSampleRate, 0.01f);
    
    windowMovement = (50 * smoothedTime.getNextValue());
}

void pseudoTS::processBlock(juce::AudioBuffer<float>& inBuffer){
    
    int splits = smoothedSplits.getNextValue();
    
    // this needs to be changed of course. it should be a variable.
    int repeats = smoothedRepeats.getNextValue();
    
    int smoothed_window_size = maxWindowSize * smoothedTime.getNextValue();
    
    // creating dynamic array that updates everysample
    // could use a vector here but i feel like doing the memory manually is much safer for audio.
    int* splitArray = new int[splits];
    //
    // this assigns our size for the loop of buffer reallocation.
    memory_loopBuffer = new juce::AudioBuffer<float>[repeats + 1];
    
    fillArray(splits, smoothed_window_size, splitArray);
    //
    //
    // THE ARRAY IS FILLED. NOW APPLY TO DSP
    // ps. the array is giving the starting points of everything.
    // NOT the whole window.
    //
    int splitWindowSize = splitArray[1] - splitArray[0];
    // ^^^^^^^
    // this allows us to automatically have the new split window size every sample.
    int num_samples = inBuffer.getNumSamples();
    
    float* in_channel_left = inBuffer.getWritePointer(0);
    float* in_channel_right = inBuffer.getWritePointer(1);
    
    float* circular_channel_left = circularBuffer.getWritePointer(0);
    float* circular_channel_right = circularBuffer.getWritePointer(1);
    
    
    for(int i = 0; i < num_samples; i++){
        //
        
        float input_left = in_channel_left[i];
        float input_right = in_channel_right[i];
        
        //
        in_channel_left[i] = 0.f;
        in_channel_right[i] = 0.f;
        
        circular_channel_left[writehead] = input_left;
        circular_channel_right[writehead] = input_right;
        
        float wet_amount = smoothedDryWet.getNextValue();
        float dry_amount = 1.f - wet_amount;
        
        if(windowComplete == false){
            in_channel_left[i] = input_left;
            in_channel_right[i] = input_right;
        }
        if (windowCounter >= smoothed_window_size){
            // the window movement is what prevents it from just reading a window at a time.
            // by having the window move it creates the pseudo time stretch.
            beginhead = writehead - smoothed_window_size;
            endhead = writehead - windowMovement;
            
            if(beginhead < 0){
                beginhead += circularBuffer.getNumSamples();
            }
            readhead = beginhead;
            windowComplete = true;
        }
        if(windowComplete == true){
            
            float stretch_out_left = circular_channel_left[readhead];
            float stretch_out_right = circular_channel_right[readhead];
            
            in_channel_left[i] = (input_left * dry_amount) +  (stretch_out_left * wet_amount);
            in_channel_right[i] = (input_right * dry_amount) + (stretch_out_right * wet_amount);
            
            readhead++;
            if(readhead < 0){
                readhead += circularBuffer.getNumSamples();
            }
        }
        
        writehead++;
        windowCounter++;
        if(writehead >= circularBuffer.getNumSamples()){
            writehead = 0;
        }
        if(windowCounter >= smoothed_window_size + 1){
            windowCounter = 0;
        }
        if(readhead > endhead){
            windowComplete = false;
        }
    }
    delete[] splitArray;
    delete[] memory_loopBuffer;
}

void pseudoTS::setParameters(float inTimePercent, float inDryWetPercent, int inSplits, int inRepeats){
    
    smoothedTime.setTargetValue(inTimePercent);
    smoothedDryWet.setTargetValue(inDryWetPercent);
    smoothedSplits.setTargetValue(inSplits);
    smoothedRepeats.setTargetValue(inRepeats);
    
}

void pseudoTS::fillArray(int inSplits, int inWindowSize, int* inArray){
    for(int i = 0; i < inSplits; i++){
        inArray[i] = (inWindowSize/inSplits) * (i + 1);
    }
}
