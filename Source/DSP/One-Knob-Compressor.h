#pragma once

#include <atomic>
#include <JuceHeader.h>

#include "../PluginParameters.h"


/**
 * @class OneKnobCompressor
 * @brief This class handles the compression functionality through a single control.
 *
 * It is a feed forward compressor with hard knee and fixed attack and release times, which are not
 * exposed to the user. The only parameter is the compression amount (0.0 - 1.0): raising it lowers
 * the threshold a little and raises the compression slope a lot, so that a single knob goes from a
 * gentle levelling to a heavy squash. A makeup gain compensates part of the resulting volume loss.
 * The level detection is linked between channels, so the same gain is applied to all of them and
 * the stereo image does not move.
 */
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
