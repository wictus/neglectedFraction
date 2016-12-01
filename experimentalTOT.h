#ifndef EXPERIMENTALTOT_H
#define EXPERIMENTALTOT_H

#include<iostream>
#include<vector>
#include<cmath>
#include<string>
#include<sstream>
#include "TRandom3.h"
#include "TH1F.h"
#include "TCanvas.h"

class experimentalTOT
{
public:
  experimentalTOT(const std::string filePath);
  
  void plotTOT();
  void setTOTThreshold(const double threshold);
  void plotTOTSpectrumWithCut();
  double calculateNeglectedFraction(); 
private:
  void plotSpectrum(const double threshold);
  double fThreshold;
  std::vector<double> fTOTs;
};

#endif // SIMULATEDCOMPTON_H
