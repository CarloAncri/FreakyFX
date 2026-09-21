#pragma once

#include <JuceHeader.h>

#include "../PluginParameters.h"


/*
 * @class DryWet
 * @brief This class handles the dry/wet mixing functionality.
 * 
 * It allows to copy the dry signal, process it on its own line (see getDryBuffer), then mixing it
 * with the wet signal, with a given dry/wet ratio.
 * The dry signal passes through a delay line to avoid phasing issues when the dry and wet signals are mixed together.
 * The delay time can be set in samples, and the dry/wet ratio can be set as float (0.0 - 1.0): 0.0 = dry, 1.0 = wet.
 */
class DryWet
{
public:
  // =================================================================
  DryWet(float defaultDW = Parameters::defaultDryWetAmount);
  ~DryWet();
  // =================================================================
  void prepareToPlay(double sampleRate, int maxBlockSize, int numChannels);
  void releaseResources();
  // =================================================================
  void copyDrySignal(juce::AudioBuffer<float> &sourceBuffer);
  void mixDrySignal(juce::AudioBuffer<float> &destinationBuffer);
  void setDWRatio(float newValue);
  void setDelaySamples(float newDelaySamples);

  juce::AudioBuffer<float> &getDryBuffer() { return drySignal; }
  // =================================================================
private:
  void updateState();

  juce::AudioBuffer<float> drySignal;
  juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine {4096};

  int numChannels = 0;
  float dwRatio = 0.5f;

  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> dryLevel;
  juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> wetLevel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)
};
