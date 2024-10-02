/*
  ==============================================================================

    psuedoTS.h
    Created: 3 Sep 2024 12:40:32pm
    Author:  Jake Morgan

  ==============================================================================
*/


// ===================================================
/*
this is how i'm gonna make this work.
 pseudo time stretching is a method in which audio is split into x amount of times and each split is played back y amount of times.
 x stands for the amount of splits
 y stands for the amount of repeats.
 if you want your stretch to double in size, play every part twice. triple, trice.
 in order to make this accomadate more than just doubling and tripling and so on, we create a window size for it to repeat.
 
 so now the formula looks like newAudio = ((source/z)/x)*y
THIS FORMULA NEEDS TO OCCUR BEFORE THE BIG FOR LOOP


*/
// ==================================================


#pragma once
#include "JuceHeader.h"

class pseudoTS
{
public:
    pseudoTS();
    
    ~pseudoTS();
    
    
    void prepareToPlay(float inSampleRate);
    
    void processBlock(juce::AudioBuffer<float>& inBuffer);
    
    void setParameters(float inTimePercent, float inDryWetPercent, int inSplits, int inRepeats);
    
    void fillArray(int inSplits, int inWindowSize, int* inArray);
    
    
private:
    
    juce::LinearSmoothedValue<float> smoothedTime;
    juce::LinearSmoothedValue<float> smoothedDryWet;
    juce::LinearSmoothedValue<int> smoothedSplits;
    juce::LinearSmoothedValue<int> smoothedRepeats;
    
    juce::AudioBuffer<float> circularBuffer;
    
    //play head
    int writehead = 0;
    int readhead = 0;
    int endhead = 0;
    int beginhead = 0;

    
    float sampleRate = 44100.f;
    
    int windowCounter = 0;
    bool windowComplete = false;
    int windowMovement = 0;
   // this should allow for us to use the dynamicness of a vector in order to make this work.
    std::vector<int> windowSplit;
    
    // this needs to be changed to be reading information from the daw and being time synced
    
    // right now the windows are too big
    const int maxWindowSize = sampleRate/ 150;
    
};
