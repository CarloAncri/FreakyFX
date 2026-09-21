#include "One-Knob-Compressor.h"


static const float attackTime = 0.015f;
static const float releaseTime = 0.200f;
static const float minLevelDb = -80.0f;


OneKnobCompressor::OneKnobCompressor(float initAmount) : targetAmount(initAmount), amount(initAmount)
{
  updateParameters();
  updateCoefficients();
}


void OneKnobCompressor::prepareToPlay(double newSampleRate)
{
  sampleRate = newSampleRate;
  gainDb = 0.0f;
  updateCoefficients();
}


void OneKnobCompressor::setAmount(float newAmount)
{
  targetAmount.store(newAmount);
}


void OneKnobCompressor::updateParameters()
{
  thresholdDb = -6.0f - 18.0f * amount;
  slope = 0.2f + 0.7f * amount;

  makeupGain = juce::Decibels::decibelsToGain(-thresholdDb * slope * 0.5f);
}


void OneKnobCompressor::updateCoefficients()
{
  attackCoeff = std::exp(-1.0f / (attackTime * (float) sampleRate));
  releaseCoeff = std::exp(-1.0f / (releaseTime * (float) sampleRate));
}


void OneKnobCompressor::processBlock(juce::AudioBuffer<float> &buffer)
{
  const float newAmount = targetAmount.load();
  if (newAmount != amount)
  {
    amount = newAmount;
    updateParameters();
  }

  const int numChannels = buffer.getNumChannels();
  const int numSamples = buffer.getNumSamples();

  for (int i = 0; i < numSamples; ++i)
  {
    float peak = 0.0f;
    for (int ch = 0; ch < numChannels; ++ch)
      peak = juce::jmax(peak, std::abs(buffer.getSample(ch, i)));

    const float levelDb = juce::Decibels::gainToDecibels(peak, minLevelDb);
    const float overDb = levelDb - thresholdDb;
    const float targetGainDb = overDb > 0.0f ? -overDb * slope : 0.0f;

    const float coeff = targetGainDb < gainDb ? attackCoeff : releaseCoeff;
    gainDb = coeff * gainDb + (1.0f - coeff) * targetGainDb;

    const float gain = juce::Decibels::decibelsToGain(gainDb) * makeupGain;

    for (int ch = 0; ch < numChannels; ++ch)
      buffer.getWritePointer(ch)[i] *= gain;
  }
}
