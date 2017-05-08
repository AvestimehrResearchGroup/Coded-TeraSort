#include <iostream>
#include <assert.h>
#include <mpi.h>
#include <iomanip>

#include "Master.h"
#include "Common.h"
#include "Configuration.h"
#include "PartitionSampling.h"

using namespace std;

void Master::run()
{
  if ( totalNode != 1 + conf.getNumReducer() ) {
    cout << "The number of workers mismatches the number of processes.\n";
    assert( false );
  }

  // GENERATE LIST OF PARTITIONS.
  PartitionSampling partitioner;
  partitioner.setConfiguration( &conf );
  PartitionList* partitionList = partitioner.createPartitions();


  // BROADCAST CONFIGURATION TO WORKERS
  MPI::COMM_WORLD.Bcast( &conf, sizeof( Configuration ), MPI::CHAR, 0 );
  // Note: this works because the number of partitions can be derived from the number of workers in the configuration.


  // BROADCAST PARTITIONS TO WORKERS
  for ( auto it = partitionList->begin(); it != partitionList->end(); it++ ) {
    unsigned char* partition = *it;
    MPI::COMM_WORLD.Bcast( partition, conf.getKeySize() + 1, MPI::UNSIGNED_CHAR, 0 );
  }

  
  // TIME BUFFER
  int numWorker = conf.getNumReducer();
  double rcvTime[ numWorker + 1 ];  
  double rTime = 0;
  double avgTime;
  double maxTime;


  // COMPUTE MAP TIME
  MPI::COMM_WORLD.Gather( &rTime, 1, MPI::DOUBLE, rcvTime, 1, MPI::DOUBLE, 0 );
  avgTime = 0;
  maxTime = 0;
  for( int i = 1; i <= numWorker; i++ ) {
    avgTime += rcvTime[ i ];
    maxTime = max( maxTime, rcvTime[ i ] );
  }
  cout << rank << ": MAP     | Avg = " << setw(10) << avgTime/numWorker
       << "   Max = " << setw(10) << maxTime << endl;


  // COMPUTE PACKING TIME
  MPI::COMM_WORLD.Gather( &rTime, 1, MPI::DOUBLE, rcvTime, 1, MPI::DOUBLE, 0 );
  avgTime = 0;
  maxTime = 0;
  for( int i = 1; i <= numWorker; i++ ) {
    avgTime += rcvTime[ i ];
    maxTime = max( maxTime, rcvTime[ i ] );
  }
  cout << rank << ": PACK    | Avg = " << setw(10) << avgTime/numWorker
       << "   Max = " << setw(10) << maxTime << endl;  

  
  // COMPUTE SHUFFLE TIME
  double txRate = 0;
  double avgRate = 0;
  for( unsigned int i = 1; i <= conf.getNumReducer(); i++ ) {
    MPI::COMM_WORLD.Barrier();
    MPI::COMM_WORLD.Barrier();
    MPI::COMM_WORLD.Recv( &rTime, 1, MPI::DOUBLE, i, 0 );
    avgTime += rTime;
    MPI::COMM_WORLD.Recv( &txRate, 1, MPI::DOUBLE, i, 0 );
    avgRate += txRate;
  }
  cout << rank << ": SHUFFLE | Sum = " << setw(10) << avgTime
       << "   Rate = " << setw(10) << avgRate/numWorker << " Mbps" << endl;  


  // COMPUTE UNPACK TIME
  MPI::COMM_WORLD.Gather( &rTime, 1, MPI::DOUBLE, rcvTime, 1, MPI::DOUBLE, 0 );
  avgTime = 0;
  maxTime = 0;
  for( int i = 1; i <= numWorker; i++ ) {
    avgTime += rcvTime[ i ];
    maxTime = max( maxTime, rcvTime[ i ] );
  }
  cout << rank << ": UNPACK  | Avg = " << setw(10) << avgTime/numWorker
       << "   Max = " << setw(10) << maxTime << endl;
  
  
  
  // COMPUTE REDUCE TIME
  MPI::COMM_WORLD.Gather( &rTime, 1, MPI::DOUBLE, rcvTime, 1, MPI::DOUBLE, 0 );
  avgTime = 0;
  maxTime = 0;
  for( int i = 1; i <= numWorker; i++ ) {
    avgTime += rcvTime[ i ];
    maxTime = max( maxTime, rcvTime[ i ] );
  }
  cout << rank << ": REDUCE  | Avg = " << setw(10) << avgTime/numWorker
       << "   Max = " << setw(10) << maxTime << endl;      
  

  // CLEAN UP MEMORY
  for ( auto it = partitionList->begin(); it != partitionList->end(); it++ ) {
    delete [] *it;
  }
}
