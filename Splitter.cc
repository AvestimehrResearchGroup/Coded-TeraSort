#include <iostream>

#include "Configuration.h"
#include "CodedConfiguration.h"
#include "InputSplitter.h"

using namespace std;

int main( int argc, char* argv[] )
{
  Configuration uncodedConf;
  CodedConfiguration codedConf;
  Configuration* conf;

  if( argc <= 1 ) {
    conf = &uncodedConf;
    cout << "Split data specified in UNCODE configuration\n";
  }
  else {
    conf = &codedConf;
    cout << "Split data specified in CODED configuration\n";
  }
  
  // SPLIT INPUT FILE TO N (ROUGHLY) EQUALLY FILES, WHERE N IS THE NUMBER OF WORKERS.
  cout << ": ----------\n";
  cout << ": Split input file\n";  
  InputSplitter inputSplitter;
  
  inputSplitter.setConfiguration( conf );
  inputSplitter.splitInputFile();
  cout << ":Done\n\n";

  return 0;
}


