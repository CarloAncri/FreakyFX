#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "PluginParameters.h"
#include "GUI/MyTheme.h"
#include "GUI/Meter.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

struct SliderPosition
{
  float x;
  float y;
  float width;
  float height;
};

#define BIG_SLIDER_DIM 100
#define MEDIUM_SLIDER_DIM 90
#define SMALL_SLIDER_DIM 60
#define METER_WIDTH 30
#define PLUG_WIDTH 530
#define PLUG_HEIGHT 320
#define PADDING 20

// ====== POSITIONS =====
#define CUTOFF_FREQ_SLIDER_POSITION SliderPosition{PADDING, PADDING, MEDIUM_SLIDER_DIM, MEDIUM_SLIDER_DIM}
#define COMPRESSOR_THR_SLIDER_POSITION SliderPosition{PADDING + MEDIUM_SLIDER_DIM + PADDING, PADDING, MEDIUM_SLIDER_DIM, MEDIUM_SLIDER_DIM}
#define OCTAVER_SLIDER_POSITION SliderPosition{PADDING, PLUG_HEIGHT - (PADDING + MEDIUM_SLIDER_DIM), MEDIUM_SLIDER_DIM, MEDIUM_SLIDER_DIM}
#define DISTORTION_AMOUNT_SLIDER_POSITION SliderPosition{PADDING + MEDIUM_SLIDER_DIM + PADDING, PLUG_HEIGHT - (PADDING + MEDIUM_SLIDER_DIM), MEDIUM_SLIDER_DIM, MEDIUM_SLIDER_DIM}

#define INPUT_GAIN_SLIDER_POSITION SliderPosition{PLUG_WIDTH-(SMALL_SLIDER_DIM*2+PADDING+8), PLUG_HEIGHT-(SMALL_SLIDER_DIM+PADDING), SMALL_SLIDER_DIM, SMALL_SLIDER_DIM}
#define OUTPUT_GAIN_SLIDER_POSITION SliderPosition{PLUG_WIDTH-(SMALL_SLIDER_DIM+PADDING+2), PLUG_HEIGHT-(SMALL_SLIDER_DIM+PADDING), SMALL_SLIDER_DIM, SMALL_SLIDER_DIM}

#define MIX_SLIDER_POSITION SliderPosition{(PLUG_WIDTH)/2 -PADDING*0.7, (PLUG_HEIGHT/2) - (BIG_SLIDER_DIM/2), BIG_SLIDER_DIM, BIG_SLIDER_DIM}
// ======================


class OneBassBandAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
  OneBassBandAudioProcessorEditor(OneBassBandAudioProcessor &p, AudioProcessorValueTreeState &vts);
  ~OneBassBandAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

  void timerCallback() override;
private:
  void setupSlider(Slider& slider, SliderPosition position);

  OneBassBandAudioProcessor &audioProcessor;
  AudioProcessorValueTreeState &valueTreeState;

  Slider inputGainSlider;
  Slider outputGainSlider;
  Slider cutoffFreqSlider;
  Slider compressorThrSlider;
  Slider pitchShiftedOctaveSlider;
  Slider distortionAmountSlider;
  Slider mixSlider; // dryWet

  Meter inMeter;
  Meter outMeter;

  MyLookAndFeel myTheme;
  MyLookAndFeel octaverKnobTheme;

  std::unique_ptr<SliderAttachment> inputGainAttachment;
  std::unique_ptr<SliderAttachment> outputGainAttachment;
  std::unique_ptr<SliderAttachment> cutoffFreqAttachment;
  std::unique_ptr<SliderAttachment> compressorThrAttachment;
  std::unique_ptr<SliderAttachment> pitchShiftedOctaveAttachment;
  std::unique_ptr<SliderAttachment> distortionAmountAttachment;
  std::unique_ptr<SliderAttachment> mixAttachment;

  juce::Typeface::Ptr OBBFont;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OneBassBandAudioProcessorEditor)
};
