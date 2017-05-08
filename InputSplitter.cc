#include <iostream>
#include <fstream>
#include <assert.h>
#include <cmath>
#include <cstdio>

#include "InputSplitter.h"
#include "Common.h"

using namespace std;

InputSplitter::InputSplitter()
{
  conf = NULL;
  bufferSize = 100000000; //Byte
}

InputSplitter::~InputSplitter()
{

}

void InputSplitter::splitInputFile()
{
  if ( !conf ) {
    cout << "Configuration has not been provided.\n";
    assert( false );
  }

  ifstream inputFile( conf->getInputPath(), ios::in | ios::binary | ios::ate );
  if ( !inputFile.is_open() ) {
    cout << "Cannot open input file " << conf->getInputPath() << endl;
    assert( false );
  }
  cout << "inputFile " << conf->getInputPath() << " is open\n";

  unsigned int numInput = conf->getNumInput();
  unsigned int LineSize = conf->getLineSize();
  unsigned long long int fileSize = inputFile.tellg();
  unsigned long long int numLine = fileSize / LineSize;

  char* buff = new char[ bufferSize ];
  if ( buff == NULL ) {
    cout << "Cannot allocate buffer for splitting\n";
    assert( false );
  }

  
  // Assume that the number of lines is higher than the number of workers
  assert( numInput <= numLine );

  
  // Split input file equally except the last split that possible includes extra lines
  unsigned long long int splitSize = ( numLine / numInput ) * LineSize;
  unsigned long long int startIndex;
  unsigned long long int endIndex;

  
  // Create split_0, ..., split_(numWorder-2)
  unsigned int i;
  assert( numInput > 1 );
  for ( i = 0; i < (unsigned int) numInput-1; i++ ) {
    startIndex = i * splitSize;
    endIndex = ( i + 1 ) * splitSize - 1;
    createSplit( inputFile, i, startIndex, endIndex, buff );
  }

  
  // Create split_(numInput-1)
  startIndex = ( numInput - 1 ) * splitSize;
  endIndex = fileSize - 1;
  createSplit( inputFile, i, startIndex, endIndex, buff );

  
  // cleanup
  inputFile.close();
  delete [] buff;
}

void InputSplitter::createSplit( ifstream &inputFile, int splitNumber, unsigned long long int startIndex, unsigned long long int endIndex, char* buff )
{
  unsigned long long int currIndex = startIndex;
  unsigned long long int copySize = 0;
  char splitPath[ MAX_FILE_PATH ];  

  // Open splitFile with path name "inputPath_splitNumber"
  sprintf( splitPath, "%s_%d", conf->getInputPath(), splitNumber );
  ofstream splitFile( splitPath, ios::out | ios::binary | ios::trunc );
  if ( !splitFile.is_open() ) {
    cout << "Cannot open split file " << splitPath << endl;
    assert( false );
  }  

  inputFile.seekg( startIndex );
  while ( currIndex <= endIndex ) {
    copySize = min( bufferSize, endIndex - currIndex + 1 );
    inputFile.read( buff, copySize );
    splitFile.write( buff, copySize );
    currIndex += copySize;
  }

  splitFile.close();

  cout << "Create " << splitPath << endl;
}
