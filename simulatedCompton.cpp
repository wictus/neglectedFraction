#include "simulatedCompton.h"

simulatedCompton::simulatedCompton(const double eRes)
:fERes(eRes)
{

}

void simulatedCompton::produceSIMEvents(const double initialEnergy)
{
    if( fSIMEvents.size()!= 0)
      fSIMEvents.clear();
        // DEKLARACJE ZMIENNYCH
        double przekroj, E_0, E, stala, fi, firad, pi;
        int i;
        double T, kos, x, y, max, Eprim;
        TRandom3 los;
        double rozdzielczosc, g, sigma, rozmyciex, Troz;

        // INICJALIZACJIA
         przekroj = 0.0;
        E = 1.0;
        stala = pow((2.82*1E-13),2.0) * 0.5;
        E_0 = initialEnergy;
        fi = 0.0;
        i = 0;
        firad=0;
        pi = 3.14159265358979323846;
        kos = 0.0;
        T = 0.0;
        Eprim = 0.0;
        rozdzielczosc =0.0;
        g= 0.0;
        sigma = 0.0;
        rozmyciex =0.0;
        Troz = 0.0;     //Ek. Kin. rozmyta

        // jeden stopien to okolo 0.0175 radiana  bo 1* pi /180 = 0.0175
        rozdzielczosc = fERes;

        fi=0;

        firad = fi * pi/180.0;
        E = E_0 * 511.0 /(511.0 + E_0*( 1- cos(firad)));

        max = E * (2.0*E/511.0)/(1.0+2.0*E/511.0);

        //// PETLA DO RYSOWANIA WIDMA

        for (i=0;i<1E6;) {

	  if(i%100000==0)
	    std::cout<<"Processing event: " <<i <<std::endl;
	
        x = los.Rndm()*max;

        y = los.Rndm()*10.0;
        g = los.Gaus(0.0,1.0);
        T = x;
        kos = (511.0/E * T + T - E)/(T-E);
        Eprim = E - T;

        przekroj =  2.0 * 3.14 * stala * pow(Eprim/E,2.0) *(E/Eprim + Eprim/E - 1.0 + pow(kos,2.0) ) * ((511.0/E + 1.0)/(T-E)- (511.0/E * T + T - E)/(pow((T-E),2.0)))*(-1.0)*1E27;



        if(przekroj>y){

        sigma = rozdzielczosc*sqrt(x);
        rozmyciex= sigma * g;
        Troz = x + rozmyciex;
        fSIMEvents.push_back(Troz);
        i++;
        }


        }
}

void simulatedCompton::plotSimulatedSpectrum()
{
  plotSpectrum(0);
}

void simulatedCompton::plotSpectrum(const double threshold)
{
  std::string title = "SimulatedCompton";
  std::stringstream buffer;
  buffer << threshold;
  title+=buffer.str();
  title+="keVCut";
  
  TH1F* histo = new TH1F(title.c_str(),title.c_str(),1500, 0, 1500);
  
  for(unsigned int i = 0; i < fSIMEvents.size(); i++)
  {
    if(fSIMEvents[i] > threshold)
      histo->Fill(fSIMEvents[i]);
  }
  TCanvas* c = new TCanvas();
  histo->GetXaxis()->SetTitle("Energy [kev]");
  histo->GetYaxis()->SetTitle("Counts");
  
  histo->Draw();
  
  std::string name = "SimulatedComptonEres";
  std::stringstream buf;
  buf << fERes << "WithCut"<<threshold<<"keV";
  name += buf.str() + ".png";
  
  c->SaveAs(name.c_str());
  delete histo;
}


void simulatedCompton::setEnergyResolution(const double eRes)
{
  fERes = eRes;
}

void simulatedCompton::setEnergyThreshold(const double threshold)
{
  fThreshold = threshold;
}

void simulatedCompton::plotSimulateSpectrumWithCut()
{
  plotSpectrum(fThreshold);
}

double simulatedCompton::calculateSurvivalFraction() const
{
  double counter =0.0;
  for(unsigned int i = 0; i < fSIMEvents.size(); i++)
  {
      if(fThreshold < fSIMEvents[i])
	counter++;
  }
  return counter/(double)fSIMEvents.size();
}


double simulatedCompton::calculateNeglectedFraction()
{
  double counter =0.0;
  for(unsigned int i = 0; i < fSIMEvents.size(); i++)
  {
      if(fThreshold > fSIMEvents[i])
	counter++;
  }
  return counter/(double)fSIMEvents.size();
}

void simulatedCompton::applyAttenuation(const double attenuationCoefficient, const double thickness)
{
  TRandom3 los;
  std::vector<double> copyOfEvents;
  std::cout<<"Probability of interaction is equal to: " << 1.0 - exp( -1.0 * attenuationCoefficient * thickness ) << std::endl;
  for( unsigned int i = 0; i < fSIMEvents.size(); i++)
  {
    if(i%100000==0)
	    std::cout<<"Processing event: " <<i <<std::endl;
      double x = los.Rndm();
      if( x < (1.0 - exp( -1.0 * attenuationCoefficient * thickness ) ) )
	copyOfEvents.push_back(fSIMEvents[i]);
      
  }
  fSIMEvents = copyOfEvents;
}

std::vector< double >& simulatedCompton::getSIMEvents()
{
  return fSIMEvents;
}


simulatedCompton simulatedCompton::operator+=(simulatedCompton comptonOne)
{
  const std::vector<double> eventsToCopy = comptonOne.getSIMEvents();
  for(unsigned int i = 0; i< eventsToCopy.size();i++)
  {
      this->getSIMEvents().push_back( comptonOne.getSIMEvents()[i] );
  }
  return *this;
}
