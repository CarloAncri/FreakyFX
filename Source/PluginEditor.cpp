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

  this->setLookAndFeel(&myTheme);
  octaverKnobTheme.setNumThicks(5);
  pitchShiftedOctaveSlider.setLookAndFeel(&octaverKnobTheme);

  setSize(PLUG_WIDTH, PLUG_HEIGHT);
}

OneBassBandAudioProcessorEditor::~OneBassBandAudioProcessorEditor()
{
  this->setLookAndFeel(nullptr);
}

//==============================================================================
void OneBassBandAudioProcessorEditor::paint(juce::Graphics &g)
{
  g.fillAll(juce::Colours::black);

  g.setColour(juce::Colours::orange);
  g.setFont(juce::FontOptions(15.0f));
  
  g.drawText("Cutoff [Hz]", PADDING, PADDING + MEDIUM_SLIDER_DIM, MEDIUM_SLIDER_DIM, 20, juce::Justification::centred, true);
  g.drawText("Comp Thr", PADDING + MEDIUM_SLIDER_DIM + PADDING, PADDING + MEDIUM_SLIDER_DIM, MEDIUM_SLIDER_DIM, 20, juce::Justification::centred, true);
  g.drawText("Octaver", PADDING, PLUG_HEIGHT - (PADDING + MEDIUM_SLIDER_DIM + PADDING + 8), MEDIUM_SLIDER_DIM, 20, juce::Justification::centred, true);
  g.drawText("Distortion", PADDING + MEDIUM_SLIDER_DIM + PADDING, PLUG_HEIGHT - (PADDING + MEDIUM_SLIDER_DIM + PADDING + 8), MEDIUM_SLIDER_DIM, 20, juce::Justification::centred, true);
  
  g.drawText("IN", PLUG_WIDTH-(SMALL_SLIDER_DIM*2+PADDING+8), PLUG_HEIGHT-(SMALL_SLIDER_DIM+PADDING*2)-5, SMALL_SLIDER_DIM, 25, juce::Justification::centred, true);
  g.drawText("OUT", PLUG_WIDTH-(SMALL_SLIDER_DIM+PADDING+2), PLUG_HEIGHT-(SMALL_SLIDER_DIM+PADDING*2)-5, SMALL_SLIDER_DIM, 25, juce::Justification::centred, true);

  g.drawText("MIX", (PLUG_WIDTH)/2 -PADDING*0.7, (PLUG_HEIGHT/2) + (BIG_SLIDER_DIM/2), BIG_SLIDER_DIM, 20, juce::Justification::centred, true);

  // =============================================================================

  // rectangle border
  g.setColour(juce::Colours::orange);
  auto bounds = getLocalBounds().toFloat();
  float cornerSize = 10.0f;
  float thick = 2.0f;
  auto borderArea = bounds.reduced (thick / 2.0f);
  g.drawRoundedRectangle(borderArea, cornerSize, thick);

  // rectangle cutoff - compressor
  g.setColour (juce::Colours::orange);
  auto cutoffCompressArea = juce::Rectangle<float>(PADDING-5, PADDING-5, (PADDING)*2+(MEDIUM_SLIDER_DIM*2)-10, (PLUG_HEIGHT/2)-(PADDING+15));
  g.drawRoundedRectangle(cutoffCompressArea, cornerSize, thick);

  // rectangle octaver - distortion
  auto octDistArea = juce::Rectangle<float>(PADDING-5, PLUG_HEIGHT-(PADDING+MEDIUM_SLIDER_DIM+PADDING+14), (PADDING)*2+(MEDIUM_SLIDER_DIM*2)-10, (PLUG_HEIGHT/2)-(PADDING+10));
  g.drawRoundedRectangle(octDistArea, cornerSize, thick);

  // mix knob border
  auto mixArea = juce::Rectangle<float>(((PLUG_WIDTH)/2 -PADDING*0.7) - (PADDING/2), (PLUG_HEIGHT/2) - (BIG_SLIDER_DIM/2) - (PADDING/2), BIG_SLIDER_DIM+PADDING, BIG_SLIDER_DIM+PADDING*2);
  g.drawRoundedRectangle(mixArea, cornerSize, thick);

  // meter border
  auto meterArea = juce::Rectangle<float>(PLUG_WIDTH - (SMALL_SLIDER_DIM*2 + PADDING + (PADDING-5)), PADDING -5, SMALL_SLIDER_DIM*2 + PADDING, PLUG_HEIGHT - (PADDING-5)*2);
  g.drawRoundedRectangle(meterArea, cornerSize, thick);
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
