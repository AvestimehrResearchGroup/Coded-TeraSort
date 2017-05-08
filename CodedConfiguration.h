#ifndef _CMR_CONFIGURATION
#define _CMR_CONFIGURATION

#include "Configuration.h"

class CodedConfiguration : public Configuration {

 private:
  unsigned int load;
  
 public:
 CodedConfiguration(): Configuration() {
    /* numInput = 12870;    // N is assumed to be K choose r      */
    /* numReducer = 16;  // K */
    /* load = 8;        // r */

    numInput = 924;    // N is assumed to be K choose r     
    numReducer = 12;  // K
    load = 6;        // r    
    
    inputPath = "./Input/Input100000-C";
    outputPath = "./Output/Output100000-C";
    partitionPath = "./Partition/Partition100000-C";
    numSamples = 10000;    
  }
  ~CodedConfiguration() {}

  unsigned int getLoad() const { return load; }
};

#endif
