#include <iostream>
#include <iomanip>

#include "Utility.h"
#include "Common.h"


void printKey( const unsigned char* key, unsigned int size )
{
  for ( unsigned long int i = 0; i < size; i++ ) {
    cout << setw( 2 ) << setfill( '0' ) << hex << (int) key[ i ] << " ";
  }
  cout << dec;
}


bool cmpKey( const unsigned char* keyA, const unsigned char* keyB, unsigned int size )
{
  for ( unsigned long i = 0; i < size; i++ ) {
    if ( keyA[ i ] < keyB[ i ] ) {
      return true;
    }
    else if ( keyA[ i ] > keyB[ i ] ) {
      return false;
    }
  }
  return false;
}
