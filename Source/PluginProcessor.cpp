#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FreakyFXAudioProcessor::FreakyFXAudioProcessor() : AudioProcessor(BusesProperties()
  .withInput("Input", juce::AudioChannelSet::stereo(), true)
  .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
  apvts(*this, nullptr, "PARAMETERS", Parameters::createParameterLayout()),
  inputGain{Parameters::defaultInputGain},
  octaver{Parameters::defaultPitchShiftedOctave},
  distortion{Parameters::defaultDistortionAmount},
  lpFilter{Parameters::defaultCutoffFreq},
  compressor{Parameters::defaultCompressorAmount},
  outputGain{Parameters::defaultOutputGain},
  dryWet{Parameters::defaultDryWetAmount}
{
  Parameters::addListenerToAllParameters(apvts, this);
}

FreakyFXAudioProcessor::~FreakyFXAudioProcessor()
{
}


bool FreakyFXAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  const auto& mainIn = layouts.getMainInputChannelSet();
  const auto& mainOut = layouts.getMainOutputChannelSet();

  if (mainIn.isDisabled() || mainOut.isDisabled())
    return false;

  if (mainIn != mainOut)
    return false;

  if (mainIn == juce::AudioChannelSet::mono() || mainIn == juce::AudioChannelSet::stereo())
    return true;

  return false;
}

//==============================================================================
const juce::String FreakyFXAudioProcessor::getName() const
{
  return JucePlugin_Name;
}

bool FreakyFXAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool FreakyFXAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool FreakyFXAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double FreakyFXAudioProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int FreakyFXAudioProcessor::getNumPrograms()
{
  return 1;
}

int FreakyFXAudioProcessor::getCurrentProgram()
{
  return 0;
}

void FreakyFXAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String FreakyFXAudioProcessor::getProgramName(int index)
{
  return {};
}

void FreakyFXAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
}

//==============================================================================
void FreakyFXAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  int numChannels = getTotalNumInputChannels();

  inputGain.prepareToPlay(sampleRate);
  octaver.prepareToPlay(sampleRate, numChannels);
  
  lpFilter.prepareToPlay(sampleRate, numChannels);
  compressor.prepareToPlay(sampleRate);

  delaySamples = sampleRateManager.prepareToPlay(sampleRate, TARGET_SAMPLE_RATE, samplesPerBlock, numChannels);
  setLatencySamples(std::round(delaySamples));
  dryWet.prepareToPlay(sampleRate, samplesPerBlock, numChannels);
  dryWet.setDelaySamples(delaySamples);

  outputGain.prepareToPlay(sampleRate);
}

void FreakyFXAudioProcessor::releaseResources()
{
  dryWet.releaseResources();
  octaver.releaseResources();
  sampleRateManager.reset();
}

void FreakyFXAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
  juce::ScopedNoDenormals noDenormals;
  juce::ignoreUnused(midiMessages);

  inputGain.processBlock(buffer);
  calculateLevel(buffer, true);
  dryWet.copyDrySignal(buffer);

  // dry line: low pass filter + compressor
  auto &dry = dryWet.getDryBuffer();
  lpFilter.processBlock(dry);
  compressor.processBlock(dry);

  // wet line: octaver + distortion
  octaver.processBlock(buffer);
  sampleRateManager.processBlock(buffer, [this](juce::dsp::AudioBlock<float> &block) {
    distortion.processBlock(block);
  });

  dryWet.mixDrySignal(buffer);
  outputGain.processBlock(buffer);
  calculateLevel(buffer, false);
}

void FreakyFXAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{ 
  if (parameterID == Parameters::dryWetAmount)
    dryWet.setDWRatio(newValue);

  if (parameterID == Parameters::pitchShiftedOctave)
    octaver.setOctave(static_cast<int>(newValue));

  if (parameterID == Parameters::distortionAmount)
    distortion.setDistortionAmount(newValue);

  if (parameterID == Parameters::cutoffFreq)
    lpFilter.setCutoffFreq(newValue);

  if (parameterID == Parameters::compressorAmount)
    compressor.setAmount(newValue);

  if (parameterID == Parameters::inputGain)
    inputGain.setGainDb(newValue);

  if (parameterID == Parameters::outputGain)
    outputGain.setGainDb(newValue);
  
}

//==============================================================================
bool FreakyFXAudioProcessor::hasEditor() const
{
  return true;
}

juce::AudioProcessorEditor *FreakyFXAudioProcessor::createEditor()
{
  return new FreakyFXAudioProcessorEditor(*this, apvts);
}

void FreakyFXAudioProcessor::calculateLevel(juce::AudioBuffer<float> &buffer, bool isInput)
{
  float maxRms = 0.0f;
  for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
  {
    float rms = buffer.getRMSLevel(ch, 0, buffer.getNumSamples());
    if (rms > maxRms) 
      maxRms = rms;
  }
    
  if (isInput) 
    sigInLvl.store(maxRms);
  else
    sigOutLvl.store(maxRms);
}

//==============================================================================
void FreakyFXAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void FreakyFXAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
  return new FreakyFXAudioProcessor();
}
