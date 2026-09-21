#include "LPFilter.h"
#include "juce_dsp/juce_dsp.h"


LPFilter::LPFilter(float initCutoffFreq) : targetCutoffFreq(initCutoffFreq),
cutoffFreq(initCutoffFreq)
{
  updateCoefficients();
}


void LPFilter::prepareToPlay(double newSampleRate, int numChannels)
{
  sampleRate = newSampleRate;
  z1.assign((size_t) numChannels, 0.0f);
  updateCoefficients();
}


void LPFilter::setCutoffFreq(float newCutoffFreq)
{
  targetCutoffFreq.store(newCutoffFreq);
}


void LPFilter::updateCoefficients()
{
  a1 = std::exp(-2.0f * juce::MathConstants<float>::pi * cutoffFreq / (float) sampleRate);
  b0 = 1.0f - a1;
}


void LPFilter::processBlock(juce::AudioBuffer<float> &buffer)
{
  const float newCutoffFreq = targetCutoffFreq.load();
  if (newCutoffFreq != cutoffFreq)
  {
    cutoffFreq = newCutoffFreq;
    updateCoefficients();
  }

  const size_t numChannels = juce::jmin((size_t) buffer.getNumChannels(), z1.size());
  const size_t numSamples = (size_t) buffer.getNumSamples();

  for (size_t ch = 0; ch < numChannels; ++ch)
  {
    float *channelData = buffer.getWritePointer((int) ch);
    float y = z1[ch];

    for (size_t i = 0; i < numSamples; ++i)
    {
      y = b0 * channelData[i] + a1 * y;
      channelData[i] = y;
    }

    z1[ch] = y;
  }
}
