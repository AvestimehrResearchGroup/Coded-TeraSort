#include <iostream>
#include <fstream>

#include "Configuration.h"

using namespace std;

class PartitionSampling {

private:
  const Configuration& conf;
  
  
public:
  PartitionSampling();
  ~PartitionSampling();

  void setConfiguration( const Configuration& configuration ) { conf = configuration; }
  void createPartitions();
};
