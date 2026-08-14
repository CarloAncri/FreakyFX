#pragma once

#include <JuceHeader.h>


/**
 * @brief A custom look and feel for the application.
 * 
 * This class modifies the appearance of knob-sliders.
 */
class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
  MyLookAndFeel();

  void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override;
  void setNumThicks(int newValue);
private:

  int numTicks = 21;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLookAndFeel)
};
