#ifndef _CMR_MASTER
#define _CMR_MASTER

#include "CodedConfiguration.h"

class CodedMaster
{
 private:
  CodedConfiguration conf;
  unsigned int rank;
  unsigned int totalNode;

 public:
 CodedMaster( unsigned int _rank, unsigned int _totalNode ): rank( _rank ), totalNode( _totalNode ) {};
  ~CodedMaster() {};

  void run();
};

#endif
