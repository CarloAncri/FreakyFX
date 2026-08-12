#pragma once
#include <JuceHeader.h>

class Meter : public juce::Component
{
public:
  Meter() = default;
  ~Meter() override = default;

  void updateLevel(float newRawLevel);

  void paint(juce::Graphics &g) override;
private:
  float sigLvl = 0.0f;
  const float alphaDecay = 0.85f;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Meter)
};