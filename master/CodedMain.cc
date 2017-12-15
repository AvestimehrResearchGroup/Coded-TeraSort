#include <iostream>
#include <mpi.h>

#include "CodedMaster.h"
#include "CodedWorker.h"

using namespace std;

int main()
{
  MPI::Init();
  int nodeRank = MPI::COMM_WORLD.Get_rank();
  int nodeTotal = MPI::COMM_WORLD.Get_size();

  if ( nodeRank == 0 ) {
    CodedMaster masterNode( nodeRank, nodeTotal );
    MPI::COMM_WORLD.Split( 0, nodeRank );
    masterNode.run();
  }
  else {
    CodedWorker workerNode( nodeRank );
    MPI::Intracomm workerComm = MPI::COMM_WORLD.Split( 1, nodeRank );
    workerNode.setWorkerComm( workerComm );
    workerNode.run();
  }

  MPI::Finalize();
  
  return 0;
}
