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
  

}

