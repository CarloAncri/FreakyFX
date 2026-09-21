#pragma once

#include <vector>
#include <atomic>
#include <JuceHeader.h>

#include "../PluginParameters.h"


class LPFilter
{
public:
  LPFilter(float initCutoffFreq = Parameters::defaultCutoffFreq);
  ~LPFilter() = default;

  void prepareToPlay(double newSampleRate, int numChannels);
  void setCutoffFreq(float newCutoffFreq);

  void processBlock(juce::AudioBuffer<float> &buffer);

private:
  void updateCoefficients();

  std::atomic<float> targetCutoffFreq;
  float cutoffFreq;
  double sampleRate = 44100.0;

  float a1 = 0.0f;
  float b0 = 1.0f;

  std::vector<float> z1;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LPFilter);
};
