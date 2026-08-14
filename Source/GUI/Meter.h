#pragma once
#include <JuceHeader.h>


/**
 * @brief Displays a level meter for audio signals.
 *
 * The decay factor is costant and not adjustable.
 */
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