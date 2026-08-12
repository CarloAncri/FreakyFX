#include "Meter.h"


void Meter::updateLevel(float newRawLevel)
{
  if (newRawLevel > sigLvl)
    sigLvl = newRawLevel;
  else
    sigLvl *= alphaDecay;

  repaint();
}


void Meter::paint(juce::Graphics &g)
{
  auto bounds = getLocalBounds().toFloat();

  g.setColour(juce::Colours::darkgrey);
  g.fillRoundedRectangle(bounds, 3.0f);

  float lvlDb = juce::Decibels::gainToDecibels(sigLvl, -60.0f);
  float mappedLvl = juce::jmap(lvlDb, -60.0f, 6.0f, 0.0f, 1.0f);
  mappedLvl = juce::jlimit(0.0f, 1.0f, mappedLvl);

  float meterH = bounds.getHeight() * mappedLvl;
  auto meterFill = bounds.removeFromBottom(meterH);

  juce::ColourGradient gradient(
    juce::Colours::green, bounds.getBottomLeft(),
    juce::Colours::red, bounds.getTopLeft(), false);
  gradient.addColour(0.5, juce::Colours::yellow);

  g.setGradientFill(gradient);
  g.fillRoundedRectangle(meterFill, 3.0f);
}