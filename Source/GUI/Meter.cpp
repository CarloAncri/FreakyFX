#include "Meter.h"

void Meter::updateLevel(float newRawLevel)
{
  if (newRawLevel > sigLvl)
    sigLvl = newRawLevel;
  else
    sigLvl *= alphaDecay;

  if (newRawLevel >= clipThreshold)
  {
    isClipping = true;
    startTimer(2000);
  }

  repaint();
}


void Meter::paint(juce::Graphics &g)
{
  auto bounds = getLocalBounds().toFloat();

  auto clipArea = bounds.removeFromTop(6.0f);
  bounds.removeFromTop(3.0f);

  auto metArea = bounds;

  g.setColour(juce::Colours::darkgrey);
  g.fillRoundedRectangle(metArea, 3.0f);

  if (isClipping)
    g.setColour(juce::Colours::red);
  else
    g.setColour(juce::Colours::darkgrey.darker(0.8f));

  g.fillRoundedRectangle(clipArea, 2.0f);

  float lvlDb = juce::Decibels::gainToDecibels(sigLvl, -60.0f);
  float mappedLvl = juce::jmap(lvlDb, -60.0f, 6.0f, 0.0f, 1.0f);
  mappedLvl = juce::jlimit(0.0f, 1.0f, mappedLvl);

  float meterH = metArea.getHeight() * mappedLvl;
  auto meterFill = metArea.removeFromBottom(meterH);

  juce::ColourGradient gradient(
    juce::Colours::green, bounds.getBottomLeft(),
    juce::Colours::red, bounds.getTopLeft(), false);
  gradient.addColour(0.5, juce::Colours::yellow);

  g.setGradientFill(gradient);
  g.fillRoundedRectangle(meterFill, 3.0f);
}


void Meter::timerCallback()
{
  isClipping = false;
  stopTimer();
  repaint();
}