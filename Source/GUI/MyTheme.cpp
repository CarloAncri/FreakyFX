#include "MyTheme.h"


MyLookAndFeel::MyLookAndFeel()
{
  setColour(juce::Slider::thumbColourId, juce::Colours::orange);
  setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
  setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::grey);
  setColour(juce::Slider::trackColourId, juce::Colours::grey);
  setColour(juce::Slider::backgroundColourId, juce::Colours::black);
}