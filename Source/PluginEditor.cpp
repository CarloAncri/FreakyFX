#include "PluginEditor.h"

//==============================================================================
OneBassBandAudioProcessorEditor::OneBassBandAudioProcessorEditor(OneBassBandAudioProcessor &p, AudioProcessorValueTreeState &vts) :
  AudioProcessorEditor(&p),
  audioProcessor(p),
  valueTreeState(vts)
{
  setupSlider(cutoffFreqSlider, CUTOFF_FREQ_SLIDER_POSITION);
  setupSlider(outputGainSlider, OUTPUT_GAIN_SLIDER_POSITION);
  setupSlider(inputGainSlider, INPUT_GAIN_SLIDER_POSITION);
  setupSlider(distortionAmountSlider, DISTORTION_AMOUNT_SLIDER_POSITION);
  setupSlider(pitchShiftedOctaveSlider, OCTAVER_SLIDER_POSITION);
  setupSlider(compressorThrSlider, COMPRESSOR_THR_SLIDER_POSITION);
  setupSlider(mixSlider, MIX_SLIDER_POSITION);

  inputGainAttachment.reset(new SliderAttachment(valueTreeState, Parameters::inputGain, inputGainSlider));
  outputGainAttachment.reset(new SliderAttachment(valueTreeState, Parameters::outputGain, outputGainSlider));
  cutoffFreqAttachment.reset(new SliderAttachment(valueTreeState, Parameters::cutoffFreq, cutoffFreqSlider));
  compressorThrAttachment.reset(new SliderAttachment(valueTreeState, Parameters::compressorThr, compressorThrSlider));
  pitchShiftedOctaveAttachment.reset(new SliderAttachment(valueTreeState, Parameters::pitchShiftedOctave, pitchShiftedOctaveSlider));
  distortionAmountAttachment.reset(new SliderAttachment(valueTreeState, Parameters::distortionAmount, distortionAmountSlider));
  mixAttachment.reset(new SliderAttachment(valueTreeState, Parameters::dryWetAmount, mixSlider));

  setSize(PLUG_WIDTH, PLUG_HEIGHT);
}

OneBassBandAudioProcessorEditor::~OneBassBandAudioProcessorEditor()
{
}

//==============================================================================
void OneBassBandAudioProcessorEditor::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  g.setColour(juce::Colours::white);
  g.setFont(juce::FontOptions(15.0f));
  
  /*
  g.drawText("Cutoff Freq", 30, 30 + MEDIUM_SLIDER_DIM, 100, 20, juce::Justification::centred, true);
  g.drawText("Octaver", 30, PLUG_HEIGHT - (30 + MEDIUM_SLIDER_DIM + 20), 100, 20, juce::Justification::centred, true);
  g.drawText("Compressor Threshold", 20, 140, 100, 20, juce::Justification::centred, true);
  g.drawText("Distortion Amount", 260, 140, 100, 20, juce::Justification::centred, true);
  g.drawText("Input Gain", 20, 20, 100, 20, juce::Justification::centred, true);
  g.drawText("Output Gain", 140, 20, 100, 20, juce::Justification::centred, true);
  g.drawText("Mix", 20, 260, 100, 20, juce::Justification::centred, true);
  */
}

void OneBassBandAudioProcessorEditor::resized()
{
}


void OneBassBandAudioProcessorEditor::setupSlider(Slider& slider, SliderPosition position)
{
  slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
  slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 80, 20);
  addAndMakeVisible(slider);
  slider.setBounds(position.x, position.y, position.width, position.height);
}
