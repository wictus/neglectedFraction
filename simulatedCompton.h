#ifndef SIMULATEDCOMPTON_H
#define SIMULATEDCOMPTON_H

#include<iostream>
#include<vector>
#include<cmath>
#include<string>
#include<sstream>
#include "TRandom3.h"
#include "TH1F.h"
#include "TCanvas.h"

class simulatedCompton
{
public:
  simulatedCompton(const double eRes);
  void produceSIMEvents(const double initialEnergy = 511.0);
  void applyAttenuation(const double attenuationCoefficient, const double thickness);
  void plotSimulatedSpectrum();
  void setEnergyResolution(const double eRes);
  void setEnergyThreshold(const double threshold);
  void plotSimulateSpectrumWithCut();
  double calculateNeglectedFraction();
  simulatedCompton operator+=(simulatedCompton compton);
  std::vector<double>& getSIMEvents(); 
  double calculateSurvivalFraction() const;
private:
  void plotSpectrum(const double threshold);
  double fERes, fThreshold;
  std::vector<double> fSIMEvents;
};

#endif // SIMULATEDCOMPTON_H
