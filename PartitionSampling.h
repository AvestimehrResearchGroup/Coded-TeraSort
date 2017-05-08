#ifndef _MR_PARTITIONSAMPLING
#define _MR_PARTITIONSAMPLING

#include <vector>
#include "Configuration.h"
#include "Common.h"

using namespace std;

class PartitionSampling {
 private:
  const Configuration* conf;  
  
 public:
  PartitionSampling();
  ~PartitionSampling();

  void setConfiguration( const Configuration* configuration ) { conf = configuration; }
  PartitionList* createPartitions();

 private:
  static bool cmpKey( const unsigned char* keyl, const unsigned char* keyr );
  void printKeys( const PartitionList& keyList ) const;
};

#endif
