#ifndef _MR_MASTER
#define _MR_MASTER

#include "Configuration.h"

class Master
{
 private:
  Configuration conf;
  unsigned int rank;
  unsigned int totalNode;

 public:
 Master( unsigned int _rank, unsigned int _totalNode ): rank( _rank ), totalNode( _totalNode ) {};
  ~Master() {};

  void run();
};

#endif
