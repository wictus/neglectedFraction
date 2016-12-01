#include "experimentalTOT.h"

experimentalTOT::experimentalTOT(const std::string filePath)
{
  std::ifstream data;
  data.open(filePath.c_str());
  if(!data.good())
  {
    std::cout << "Wrong file path: " << filePath << std::endl;
    exit(1);
  }
  
  double TOT = 0;
  while( data >> TOT )
    fTOTs.push_back( TOT / 1000.0);

}

void experimentalTOT::setTOTThreshold(const double threshold)
{
  fThreshold = threshold;
}


void experimentalTOT::plotTOTSpectrumWithCut()
{
  plotSpectrum(fThreshold);
}


void experimentalTOT::plotTOT()
{
  plotSpectrum(0);
}


double experimentalTOT::calculateNeglectedFraction()
{
  double counter =0.0;
  for(unsigned int i = 0; i < fTOTs.size(); i++)
  {
      if(fThreshold > fTOTs[i])
	counter++;
  }
  return counter/(double)fTOTs.size();
}


void experimentalTOT::plotSpectrum(const double threshold)
{
  std::string title = "SimulatedCompton";
  std::stringstream buffer;
  buffer << threshold;
  title+=buffer.str();
  title+="nsCut";
  
  TH1F* histo = new TH1F(title.c_str(),title.c_str(),50, 0, 50);
  
  for(unsigned int i = 0; i < fTOTs.size(); i++)
  {
    if(fTOTs[i] > threshold)
      histo->Fill(fTOTs[i]);
  }
  TCanvas* c = new TCanvas();
  histo->GetXaxis()->SetTitle("Time Over Threshold [ns]");
  histo->GetYaxis()->SetTitle("Counts");
  
  histo->Draw();
  
  std::string name = "TOTSpectrum";
  std::stringstream buf;
  buf << "WithCut"<<threshold<<"ns";
  name += buf.str() + ".png";
  
  c->SaveAs(name.c_str());
  delete histo;
}
