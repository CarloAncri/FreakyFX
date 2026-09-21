#pragma once

#include <atomic>
#include <JuceHeader.h>

#include "../PluginParameters.h"


class OneKnobCompressor
{
public:
  OneKnobCompressor(float initAmount = Parameters::defaultCompressorAmount);
  ~OneKnobCompressor() = default;

  void prepareToPlay(double newSampleRate);
  void setAmount(float newAmount);

  void processBlock(juce::AudioBuffer<float> &buffer);

private:
  void updateParameters();
  void updateCoefficients();

  std::atomic<float> targetAmount;
  float amount;
  double sampleRate = 44100.0;

  float thresholdDb = 0.0f;
  float slope = 0.0f;          // 1 - 1/ratio
  float makeupGain = 1.0f;

  float attackCoeff = 0.0f;
  float releaseCoeff = 0.0f;

  float gainDb = 0.0f;         // current gain reduction, shared by every channel

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OneKnobCompressor);
};
