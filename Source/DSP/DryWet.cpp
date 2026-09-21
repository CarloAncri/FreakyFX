#include "DryWet.h"


DryWet::DryWet(float defaultDW) { dwRatio = defaultDW; }
DryWet::~DryWet() {}


void DryWet::prepareToPlay(double sampleRate, int maxBlockSize, int numChannels)
{
  this->numChannels = numChannels;

  drySignal.setSize(this->numChannels, maxBlockSize);
  drySignal.clear();

  dryLevel.reset(sampleRate, 0.01);
  wetLevel.reset(sampleRate, 0.01);

  // delay line (compensate for anti-aliasing filters)
  juce::dsp::ProcessSpec info;
  info.sampleRate = sampleRate;
  info.maximumBlockSize = static_cast<uint32_t>(maxBlockSize);
  info.numChannels = static_cast<uint32_t>(this->numChannels);
  delayLine.prepare(info);
  delayLine.reset();

  updateState();
}


void DryWet::releaseResources()
{
  drySignal.setSize(0, 0);
  delayLine.reset();
  numChannels = 0;
}


void DryWet::copyDrySignal(juce::AudioBuffer<float> &sourceBuffer)
{
  auto numCh = sourceBuffer.getNumChannels();
  auto numSamples = sourceBuffer.getNumSamples();
  auto realChannels = juce::jmin(numCh, drySignal.getNumChannels());

  // resize to the current block without reallocating (memory reserved in prepareToPlay)
  drySignal.setSize(drySignal.getNumChannels(), numSamples, false, false, true);

  for (int ch = 0; ch < realChannels; ++ch)
    drySignal.copyFrom(ch, 0, sourceBuffer, ch, 0, numSamples);

  juce::dsp::AudioBlock<float> block(drySignal);
  juce::dsp::ProcessContextReplacing<float> context(block);
  delayLine.process(context);
}


void DryWet::mixDrySignal(juce::AudioBuffer<float> &destinationBuffer)
{
  auto numCh = destinationBuffer.getNumChannels();
  auto numSamples = destinationBuffer.getNumSamples();
  auto realChannels = juce::jmin(numCh, drySignal.getNumChannels());

  dryLevel.applyGain(drySignal, numSamples);
  wetLevel.applyGain(destinationBuffer, numSamples);

  for (int ch = 0; ch < realChannels; ++ch)
    destinationBuffer.addFrom(ch, 0, drySignal, ch, 0, numSamples);
}


void DryWet::setDWRatio(float newValue)
{
  dwRatio = newValue;
  updateState();
}


void DryWet::setDelaySamples(float newDelaySamples)
{
  delayLine.setDelay(newDelaySamples);
}


void DryWet::updateState()
{
  dryLevel.setTargetValue(std::sqrt(1.0f - dwRatio));
  wetLevel.setTargetValue(std::sqrt(dwRatio));
}