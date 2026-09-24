#pragma once

#include <JuceHeader.h>

#include "PluginParameters.h"
#include "DSP/Octaver.h"
#include "DSP/SampleRate-Incapsulator.h"
#include "DSP/Distortion.h"
#include "DSP/LPFilter.h"
#include "DSP/One-Knob-Compressor.h"
#include "DSP/Gain-Stager.h"
#include "DSP/DryWet.h"

# define TARGET_SAMPLE_RATE 96000.0f

class FreakyFXAudioProcessor : public juce::AudioProcessor, private juce::AudioProcessorValueTreeState::Listener
{
public:
  //==============================================================================
  FreakyFXAudioProcessor();
  ~FreakyFXAudioProcessor() override;

  bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  void parameterChanged(const juce::String &parameterID, float newValue) override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  // =============================================================================
  std::atomic<float> sigInLvl{0.0f};
  std::atomic<float> sigOutLvl{0.0f};
  void calculateLevel(juce::AudioBuffer<float> &buffer, bool isInput);
private:
  juce::AudioProcessorValueTreeState apvts;

  float delaySamples = 0.0f; // delay introduced by anti-aliasing filters (SampleRateIncapsulator)

  GainStager inputGain;
  Octaver octaver;
  SampleRateIncapsulator sampleRateManager;
  Distortion distortion;
  LPFilter lpFilter;
  OneKnobCompressor compressor;
  GainStager outputGain;
  DryWet dryWet;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FreakyFXAudioProcessor)
};
