#include "Gain-Stager.h"

GainStager::GainStager(float initGainDb) : smoothedGain(juce::Decibels::decibelsToGain(initGainDb)) {}

void GainStager::prepareToPlay(double sampleRate)
{
  smoothedGain.reset(sampleRate, 0.05);
}


void GainStager::setGainDb(float newGainDb)
{
  float linearGain = juce::Decibels::decibelsToGain(newGainDb);
  smoothedGain.setTargetValue(linearGain);
}


void GainStager::processBlock(juce::AudioBuffer<float> &buffer)
{
  if (smoothedGain.isSmoothing())
  {
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    auto writePointers = buffer.getArrayOfWritePointers();

    for (int i = 0; i < numSamples; ++i)
    {
      float currentGain = smoothedGain.getNextValue();

      for (int ch = 0; ch < numChannels; ++ch)
        writePointers[ch][i] *= currentGain;
    }
  }
  else
  {
    float currentGain = smoothedGain.getCurrentValue();

    if (currentGain == 1.0f)
      return;

    buffer.applyGain(currentGain);
  }
}