#ifndef _MR_INPUTSPLITTER
#define _MR_INPUTSPLITTER

#include <fstream>
#include "Configuration.h"

using namespace std;

class InputSplitter {
 private:
  const Configuration* conf;
  unsigned long long int bufferSize;

 public:
  InputSplitter();
  ~InputSplitter();

  void setConfiguration( const Configuration* configuration ) { conf = configuration; }
  void splitInputFile();

 private:
  void createSplit( ifstream &inputFile, int splitNumber, unsigned long long int startIndex, unsigned long long int endIndex, char* buff );
};



#endif
