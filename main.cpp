#include <iostream>
#include <utility>
#include "TGraph.h"
#include "simulatedCompton.h"
#include "experimentalTOT.h"
//#include "experimentalAmplitude.h"


void plotNeglectedFraction(const std::vector<std::pair<double,double> >& data);
void simulationPart(const double resolution);
void TOTPart();
//void amplitudePart();
void runSyncPart();

int main(int argc, char **argv) {
  TOTPart();
}
  
void runSyncPart(){
  
  simulatedCompton simulationOf511(2.56939);
  simulationOf511.produceSIMEvents(); // 511, not adding
  simulationOf511.applyAttenuation(0.0965, 1.9);
  
  simulatedCompton another511(2.56939);
  another511.produceSIMEvents();
  another511.applyAttenuation(0.0965, 1.9);
  simulationOf511+=another511;
  
  simulatedCompton thirdGamma(2.56939);
  thirdGamma.produceSIMEvents(1274);
  thirdGamma.applyAttenuation( 6.163E-02,1.9);
  simulationOf511+=thirdGamma;
  simulationOf511.plotSimulatedSpectrum();
  
  std::vector<double> thresholdsInMiliVolts;
  thresholdsInMiliVolts.push_back(100);
  thresholdsInMiliVolts.push_back(150);
  thresholdsInMiliVolts.push_back(200);
  thresholdsInMiliVolts.push_back(300);
  
  std::vector<double> thresholdsInKeV;
  for(unsigned int i = 0; i < thresholdsInMiliVolts.size(); i++)
  {
      thresholdsInKeV.push_back(thresholdsInMiliVolts[i]*0.678723);
  }
  
  std::vector< std::pair<int,double> > countsPerThreshold;
  for(unsigned int i = 0; i < thresholdsInKeV.size(); i++)
  {
    simulationOf511.setEnergyThreshold(thresholdsInKeV[i]);
    double survived = simulationOf511.calculateSurvivalFraction();
    countsPerThreshold.push_back(std::make_pair<int,double>(thresholdsInMiliVolts[i], survived));
  }
  
  std::vector<double> previousRatios;
  previousRatios.push_back(300.0/180.0);
  previousRatios.push_back(280.0/180.0 );
  previousRatios.push_back(230.0/180.0);
  previousRatios.push_back(1);
  
  std::cout<< "Ratios between counts with relation to the highest threshold" <<std::endl;
  for(unsigned int i = 0; i < countsPerThreshold.size(); i++)
  {
      std::cout<<"Threshold: " << countsPerThreshold[i].first << " ratio: \t" << countsPerThreshold[i].second / countsPerThreshold.back().second << std::endl;
      std::cout<<"Previous ratio \t\t " << previousRatios[i] << std::endl;
  }
}


void TOTPart()
{
  experimentalTOT tot("TOTs54.txt");
  tot.plotTOT();
  std::vector<std::pair<double,double> > data;
  for(double cut = 0; cut < 10;cut+=0.0025)
  {
    tot.setTOTThreshold(cut);
    data.push_back( std::make_pair<double, double>(cut, tot.calculateNeglectedFraction() ) );
    if(tot.calculateNeglectedFraction() < 0.565 && tot.calculateNeglectedFraction() > 0.555)
    {
      std::cout<< "For " << tot.calculateNeglectedFraction() << " neglected we have cut equal to:" << cut << std::endl;
     tot.plotTOTSpectrumWithCut();
    }
  }
  
  plotNeglectedFraction(data);
 
}


void plotNeglectedFraction(const std::vector< std::pair< double, double > >& data)
{
  std::vector<double> x,y;
  
  std::cout<< x.size() << std::endl;
  
  for(unsigned int i = 0; i < data.size(); i++)
  {
      x.push_back(data[i].first);
      y.push_back(data[i].second);
  }
  std::cout<< x.size() << std::endl;
  
  TCanvas* c = new TCanvas();
  TGraph* graph = new TGraph(x.size(), &x[0], &y[0]);
  graph->SetMarkerStyle(21);
  graph->SetMarkerSize(0.5);
  graph->SetTitle("Neglected fraction");
  graph->Draw("AP");
  c->SaveAs("neglectedFraction.png");
  c->SaveAs("neglectedFraction.root");

}

/*
void amplitudePart()
{
  experimentalAmplitude amp("amplitudes/57_ampltudes.txt");
  amp.plotAmplitude();
  
  simulatedCompton comptonAmplitude(2.56939);
  comptonAmplitude.produceSIMEvents();
  std::vector<double> thresholds;
  thresholds.push_back(100);
  thresholds.push_back(150);
  thresholds.push_back(200);
  thresholds.push_back(300);
  
  comptonAmplitude.plotSimulatedSpectrum();
  std::vector< std::pair<double, double> > neglectedFractionsPerThreshold;
  for(unsigned int i = 0; i < thresholds.size(); i++)
  {
    comptonAmplitude.setEnergyThreshold( thresholds[i]*0.678723 );
    comptonAmplitude.plotSimulateSpectrumWithCut();
    neglectedFractionsPerThreshold.push_back( std::make_pair<>(thresholds[i], 100.0 - comptonAmplitude.calculateNeglectedFraction()*100.0 ) );
  }
  
  for(unsigned int i = 0; i < neglectedFractionsPerThreshold.size(); i++)
  {
      std::cout<< "Threshold: " << neglectedFractionsPerThreshold[i].first << " mV cuts of " << neglectedFractionsPerThreshold[i].second / neglectedFractionsPerThreshold[3].second << std::endl;
  }
} 



void simulationPart(const double resolution)
{
   simulatedCompton testSpectrum(resolution);
  testSpectrum.produceSIMEvents();
  testSpectrum.plotSimulatedSpectrum();
  
  std::vector<std::pair<double,double> > data;
  
  for(double threshold = 0; threshold < 400; threshold+=10)
  {
   testSpectrum.setEnergyThreshold(threshold); 
   data.push_back( std::make_pair<double, double>(threshold, testSpectrum.calculateNeglectedFraction() ) );
   if(threshold == 200)
     std::cout<< "For 200 keV we have fraction: " << testSpectrum.calculateNeglectedFraction() << std::endl;
  }
  
  plotNeglectedFraction(data);
}
*/