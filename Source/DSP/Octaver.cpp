#include "Octaver.h"


Octaver::Octaver(int currentOctave) : currentOctave(currentOctave), targetOctave(currentOctave)
{
  updateParameters();
}

void Octaver::prepareToPlay(double sampleRate, int numChannels)
{
  this->numChannels = numChannels;
  this->mSampleRate = sampleRate;
  this->phaseIncrement = this->modFrequency / static_cast<float>(this->mSampleRate);

  this->currentPhase = 0.0f;
  this->writePosition = 0;

  // lenght = 1 sec
  delayBuffer.setSize(this->numChannels, static_cast<int>(this->mSampleRate * 1.0));
  delayBuffer.clear();

  currentOctave = targetOctave.load();
  updateParameters();
}


void Octaver::releaseResources()
{
  delayBuffer.setSize(0, 0);
  numChannels = 0;
}


void Octaver::setOctave(int newOctave) { targetOctave.store(newOctave); }


void Octaver::updateParameters()
{
  if (currentOctave == 0)
  {
    amplitudeSamples = 0.0f;
    return;
  }

  float R = std::pow(2.0f, static_cast<float>(currentOctave));
  float T = 1.0f / modFrequency;
  float Amp_seconds = 0.0f;

  if (R > 1.0f) // pitch up
  { 
    Amp_seconds = T * (R - 1.0f);
    isPitchUp = true;
  }
  else // pitch down
  { 
    Amp_seconds = T * (1.0f - R);
    isPitchUp = false;
  }

  amplitudeSamples = Amp_seconds * static_cast<float>(mSampleRate);
}


void Octaver::processBlock(juce::AudioBuffer<float> &buffer)
{
  const int target = targetOctave.load();

  if (currentOctave != target)
  {
    currentOctave = target;
    updateParameters();
  }

  const int numChannels = buffer.getNumChannels();
  const int numSamples = buffer.getNumSamples();
  const int delayLen = delayBuffer.getNumSamples();

  // processes a max of 2 channels
  const int channelsToProcess = juce::jmin(numChannels, delayBuffer.getNumChannels());

  // === samples ===
  for (int i = 0; i < numSamples; ++i)
  {
    float s1 = currentPhase;
    float s2 = std::fmod(s1 + 0.5f, 1.0f);

    float m1 = -0.5f * std::cos(juce::MathConstants<float>::twoPi * s1) + 0.5f;
    float m2 = -0.5f * std::cos(juce::MathConstants<float>::twoPi * s2) + 0.5f;

    float effS1 = isPitchUp ? (1.0f - s1) : s1;
    float effS2 = isPitchUp ? (1.0f - s2) : s2;

    // delay samples
    float delay1 = effS1 * amplitudeSamples;
    float delay2 = effS2 * amplitudeSamples;

    float readPos1 = static_cast<float>(writePosition) - delay1;
    float readPos2 = static_cast<float>(writePosition) - delay2;

    // === channels ===
    for (int ch = 0; ch < channelsToProcess; ++ch)
    {
      float inputSample = buffer.getReadPointer(ch)[i];

      delayBuffer.setSample(ch, writePosition, inputSample);

      if (currentOctave != 0)
      {
        float out1 = getLinearSample(ch, readPos1, delayLen);
        float out2 = getLinearSample(ch, readPos2, delayLen);

        float finalSample = (out1 * m1) + (out2 * m2);

        buffer.getWritePointer(ch)[i] = finalSample;
      }
    }

    writePosition++;
    // circular buffer wrap
    if (writePosition >= delayLen)
      writePosition = 0;

    // phase wrap
    currentPhase += phaseIncrement;
    if (currentPhase >= 1.0f)
      currentPhase -= 1.0f;
  }
}


float Octaver::getLinearSample(int channel, float readPos, int bufferLength)
{
  if (readPos < 0.0f)
    readPos += static_cast<float>(bufferLength);
    
  if (readPos >= static_cast<float>(bufferLength))
    readPos -= static_cast<float>(bufferLength);

  int index = static_cast<int>(std::floor(readPos));
  float frac = readPos - static_cast<float>(index);
  int indexNext = (index + 1) % bufferLength;

  const float *data = delayBuffer.getReadPointer(channel);

  float y0 = data[index];
  float y1 = data[indexNext];

  return y0 + frac * (y1 - y0);
}