#include "Distortion.h"


Distortion::Distortion(float distAmount)
{
  setDistortionAmount(distAmount);
}


void Distortion::setDistortionAmount(float newAmount)
{
  amount = juce::jlimit(0.0f, 1.0f, newAmount);
  thr = 1.0f - (amount * 0.95f);
  drive = 1.0f / thr;
}


void Distortion::processBlock(juce::dsp::AudioBlock<float> &block)
{
  if (amount == 0.0f)
    return;

  const size_t numChannels = block.getNumChannels();
  const size_t numSamples = block.getNumSamples();

  const float asymmetry = 0.1f * amount;
  const float dcOffsetCompensation = std::tanh(asymmetry);

  for (size_t ch = 0; ch < numChannels; ++ch)
  {
    float *channelData = block.getChannelPointer(ch);

    for (size_t i = 0; i < numSamples; ++i)
    {
      float x = channelData[i];
      float drivenX = (x * drive) + asymmetry;
      float distorted = std::tanh(drivenX);
      distorted -= dcOffsetCompensation;

      channelData[i] = distorted;
    }
  }
}