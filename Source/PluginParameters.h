#pragma once
#include <JuceHeader.h>

namespace Parameters
{
  static const String inputGain = "inG";
  static const String outputGain = "ouG";
  static const String cutoffFreq = "coF";
  static const String compressorAmount = "cmA";
  static const String pitchShiftedOctave = "psO";
  static const String distortionAmount = "dsA";
  static const String dryWetAmount = "dwA";

  static const float defaultInputGain = 0.0f;
  static const float defaultOutputGain = 0.0f;
  static const float defaultCutoffFreq = 1000.0f;
  static const float defaultCompressorAmount = 0.3f;
  static const int defaultPitchShiftedOctave = 0;
  static const float defaultDistortionAmount = 0.5f;
  static const float defaultDryWetAmount = 1.0f;


  
  static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
  {
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back(std::make_unique<AudioParameterFloat>(inputGain, "Input Gain (dB)", NormalisableRange<float>(-48.0f, 12.0f, 0.1f, 1.0f), defaultInputGain));
    params.push_back(std::make_unique<AudioParameterFloat>(outputGain, "Output Gain (dB)", NormalisableRange<float>(-48.0f, 12.0f, 0.1f, 1.0f), defaultOutputGain));
    params.push_back(std::make_unique<AudioParameterFloat>(cutoffFreq, "Cutoff Frequency", NormalisableRange<float>(20.0f, 20000.0f, 0.001f, 0.25f), defaultCutoffFreq));
    params.push_back(std::make_unique<AudioParameterFloat>(compressorAmount, "Compressor Amount", NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), defaultCompressorAmount));
    params.push_back(std::make_unique<AudioParameterInt>(pitchShiftedOctave, "Pitch Shifted Octave", -2, 2, defaultPitchShiftedOctave));
    params.push_back(std::make_unique<AudioParameterFloat>(distortionAmount, "Distortion Amount", NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), defaultDistortionAmount));
    params.push_back(std::make_unique<AudioParameterFloat>(dryWetAmount, "Dry Wet Amount", NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), defaultDryWetAmount));

    return {params.begin(), params.end()};
  }



  static void addListenerToAllParameters(AudioProcessorValueTreeState &valueTreeState, AudioProcessorValueTreeState::Listener *listener)
  {
    std::unique_ptr<XmlElement> xml(valueTreeState.copyState().createXml());

    for (auto element : xml->getChildWithTagNameIterator("PARAM"))
    {
      const String &id = element->getStringAttribute("id");
      valueTreeState.addParameterListener(id, listener);
    }
  }
}