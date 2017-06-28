#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Configuration.h"
#include "CodedConfiguration.h"
#include "CodeGeneration.h"

#define BUFF_SIZE 1000000   // 1MB
#define MAX_FILE_PATH 128

using namespace std;

typedef struct FileInfo {
  unsigned int id;
  unsigned long long startByte;
  unsigned long long endByte;
} FileInfo;

int main( int argc, char* argv[] )
{
  Configuration uncodedConf;
  CodedConfiguration codedConf;
  Configuration* conf;
  bool isCode = argc > 1;
  CodeGeneration* cg = NULL;

  // Initialize configuration, which is known to all nodes
  if( !isCode ) {
    conf = &uncodedConf;
  }
  else {
    conf = &codedConf;
    cg = new CodeGeneration( conf->getNumInput(), conf->getNumReducer(), ((CodedConfiguration*) conf)->getLoad() );
  }

  // Initialize OpenMPI
  MPI::Init();
  int nodeRank = MPI::COMM_WORLD.Get_rank();
  unsigned int nodeTotal = MPI::COMM_WORLD.Get_size();

  if( nodeTotal != conf->getNumReducer() + 1 ) {
    if( nodeRank == 0 ) {
      cout << "The number of workers mismatches the number of processes.\n";
    }
    MPI::Finalize();
    return 0;    
  }

  
  // Master or Workers
  if( nodeRank == 0 ) {
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

    // // Assume that the number of lines is higher than the number of workers
    // assert( numInput <= numLine );
    // assert( numInput > 1 );

    char* buff = new char[ BUFF_SIZE ];
    if ( buff == NULL ) {
      cout << "Cannot allocate buffer for splitting\n";
      assert( false );
    }

    // Split input file equally except the last split that possible includes extra lines
    unsigned long long int splitSize = ( numLine / numInput ) * LineSize;        

    // Create infomation of original files
    FileInfo fileInfos[ numInput ]; // file id = index + 1    
    for ( unsigned int i = 0; i < numInput; i++) {
      fileInfos[ i ].id = i + 1;
      fileInfos[ i ].startByte = i * splitSize;
      fileInfos[ i ].endByte = ( i != numInput - 1 ) ? ( i + 1 ) * splitSize - 1 : fileSize - 1;
      // cout << "Original file " << fileInfos[ i ].id
      // 	   << " startByte " << fileInfos[ i ].startByte
      // 	   << " endByte " << fileInfos[ i ].endByte
      // 	   << " TotalByte " << fileInfos[ i ].endByte - fileInfos[ i ].startByte + 1
      // 	   << endl;
    }

    if( !isCode ) {
      // Uncode TX
      for ( unsigned int i = 0; i < (unsigned int) numInput; i++ ) {
	unsigned long long totalSize = fileInfos[ i ].endByte - fileInfos[ i ].startByte + 1;
	unsigned long long currIndex = fileInfos[ i ].startByte;      
	unsigned long long copySize;

	cout << "Sending file " << fileInfos[ i ].id << endl;      
	// Initial transmission
	MPI::COMM_WORLD.Send( &totalSize, 1, MPI::UNSIGNED_LONG_LONG, fileInfos[ i ].id, 0 );

	// read split and send to workers
	inputFile.seekg( currIndex );      
	while( currIndex <= fileInfos[ i ].endByte ) {
	  copySize = min( (unsigned long long) BUFF_SIZE, fileInfos[ i ].endByte - currIndex + 1 );
	  inputFile.read( buff, copySize );
	  currIndex += copySize;

	  // send to workers
	  int rxNodeId = i + 1;
	  MPI::COMM_WORLD.Send( buff, copySize, MPI::CHAR, rxNodeId, 0 );
	}
      }
    }
    else {
      // Code TX

      // Randomly assign original files to subset of nodes
      assert( codedConf.getNumReducer() <= RAND_MAX );
      srand( time(NULL) );
      map< NodeSet, vector<unsigned long> > newFileMap;
      for( unsigned long ofid = 1; ofid <= numInput; ofid++ ) {
    	NodeSet ns;
    	for( unsigned int i = 0; i < codedConf.getLoad(); i++ ) {
    	  do {
    	    ns.insert( (rand() % codedConf.getNumReducer()) + 1 );
    	  } while ( ns.size() == i );
    	}
    	newFileMap[ns].push_back( ofid );
    	cout << "Original file " << ofid << " is assigned to nodes: ";
    	for( auto nit = ns.begin(); nit != ns.end(); nit++ ) {
    	  cout << *nit << " ";
    	}
    	cout << endl;
      }

      // Multicast new files (subset-by-subset)
      MPI::Intracomm mgComm;
      for( unsigned int i = 0; i < numInput; i++ ) {
	unsigned int nfid = i + 1;
	NodeSet& ns = cg->getNodeSetFromFileID( nfid );
	int color = 1;
	mgComm = MPI::COMM_WORLD.Split( color, nodeRank );      
	cout << "Subset: ";
	for( auto nit = ns.begin(); nit != ns.end(); nit++ ) {
	  cout << *nit << " ";
	}
	// cout << endl;

	// Calculate total size of data to be sent
	unsigned long long totalSize = 0;
	for( auto oit = newFileMap[ ns ].begin(); oit != newFileMap[ ns ].end(); oit++ ) {
	  unsigned int idx = *oit - 1;
	  totalSize += fileInfos[ idx ].endByte - fileInfos[ idx ].startByte + 1;
	}
	cout << "    Total size = " << totalSize << endl;
	mgComm.Bcast( &totalSize, 1, MPI::UNSIGNED_LONG_LONG, 0 );

	// Send data

	// //inputNew is a local copy of data to be sent. It is for the purpose of debugging
	// char filePath[ MAX_FILE_PATH ];
	// sprintf( filePath, "%s_%d_S", conf->getInputPath(), nfid - 1 );
	// ofstream inputNew( filePath, ios::out | ios::binary | ios::trunc );
	// if ( !inputNew.is_open() ) {
	//   cout << "Cannot open input file " << filePath << endl;
	//   assert( false );
	// }

	if( totalSize == 0 ) {
	  // inputNew.close();
	  continue;
	}
	
	unsigned long long totalSend = 0;
	vector<unsigned long>::iterator oit = newFileMap[ ns ].begin();
	unsigned int idx = *oit - 1;
	unsigned long long currIndex = fileInfos[ idx ].startByte;
	inputFile.seekg(currIndex);
	unsigned long long copySize = 0;
	while( totalSend < totalSize ) {
	  if( currIndex > fileInfos[ idx ].endByte ) {
	    oit++;
	    idx = *oit - 1;
	    currIndex = fileInfos[ idx ].startByte;
	    inputFile.seekg( currIndex );	    
	  }

	  unsigned long long readByte = min( BUFF_SIZE - copySize, fileInfos[ idx ].endByte - currIndex + 1 );
	  inputFile.read( buff + copySize, readByte );
	  currIndex += readByte;
	  copySize += readByte;

	  if( copySize == BUFF_SIZE || totalSend + copySize == totalSize ) {
	    // inputNew.write( buff, copySize );
	    mgComm.Bcast( buff, copySize, MPI::CHAR, 0 );
	    totalSend += copySize;
	    copySize = 0;
	  }
	}
	
	// inputNew.close();
	mgComm.Free();
      }
    }

    // cleanup
    inputFile.close();
    delete [] buff;      
  }
  else {
    // Worker side (receive input files)
    char filePath[ MAX_FILE_PATH ];
    unsigned long long totalSize;
    char* buff = new char[ BUFF_SIZE ];
    unsigned long long currSize;
    unsigned long long recvSize;

    if( !isCode ) {
      // Uncode
      MPI::COMM_WORLD.Recv( &totalSize, 1, MPI::UNSIGNED_LONG_LONG, 0, 0 );
      sprintf( filePath, "%s_%d", conf->getInputPath(), nodeRank - 1 );
      ofstream inputFile( filePath, ios::out | ios::binary | ios::trunc );
      if ( !inputFile.is_open() ) {
	cout << "Cannot open input file " << filePath << endl;
	assert( false );
      }
      currSize = 0;
      while( currSize < totalSize ) {
	recvSize = min( (unsigned long long) BUFF_SIZE, totalSize - currSize );
	MPI::COMM_WORLD.Recv( buff, recvSize, MPI::CHAR, 0, 0 );
	inputFile.write( buff, recvSize );
	currSize += recvSize;
      }
      inputFile.close();
    } 
    else {
      //Code
      unsigned int numInput = conf->getNumInput();
      for( unsigned int i = 0; i < numInput; i++ ) {
	unsigned int fid = i + 1;

	// create multicast domain
	NodeSet& ns = cg->getNodeSetFromFileID( fid );
	int color = ( ns.find( nodeRank ) != ns.end() ) ? 1 : 0;
	MPI::Intracomm mgComm = MPI::COMM_WORLD.Split( color, nodeRank );
	if( color == 0 ) {
	  // No file to receive with respect to this subset
	  continue;
	}
	else {
	  mgComm.Bcast( &totalSize, 1, MPI::UNSIGNED_LONG_LONG, 0 );
	  // sprintf( filePath, "%s_%d_W%d", conf->getInputPath(), fid - 1, nodeRank ); // for local test
	  sprintf( filePath, "%s_%d", conf->getInputPath(), fid - 1 );	  
	  ofstream inputFile( filePath, ios::out | ios::binary | ios::trunc );
	  if ( !inputFile.is_open() ) {
	    cout << "Cannot open input file " << filePath << endl;
	    assert( false );
	  }
	  currSize = 0;
	  while( currSize < totalSize ) {
	    recvSize = min( (unsigned long long) BUFF_SIZE, totalSize - currSize );
	    mgComm.Bcast( buff, recvSize, MPI::CHAR, 0 );
	    inputFile.write( buff, recvSize );
	    currSize += recvSize;
	  }
	  inputFile.close();
	}
	mgComm.Free();
      }
    }
    delete [] buff;
  }
  MPI::Finalize();
  return 0;
}
