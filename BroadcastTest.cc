#include <iostream>
#include <fstream>
#include <mpi.h>
#include <ctime>
#include <assert.h>
#include <vector>

#define LINE_SIZE 100
#define INPUT_PATH "./Input/Input10000"

using namespace std;

typedef vector< unsigned char* > LineList;

void testBroadcastRoot( int, LineList&, MPI::Intracomm& );
void testBroadcast( int, MPI::Intracomm& );
void testUnicastRoot( int, LineList&, MPI::Intracomm& );
void testUnicast( int, MPI::Intracomm& );
void testScatterRoot( int, LineList&, MPI::Intracomm& );
void testScatter( int, MPI::Intracomm& );

int main( int argc, char* argv[] )
{
  clock_t time;
  LineList lineList;
  
  MPI::Init();
  MPI::Intracomm bcComm = MPI::COMM_WORLD.Dup();  
  int rank = bcComm.Get_rank();
  int numNode = bcComm.Get_size();

  LineList mlineList;
  
  // Prepare inputlist
  // For node 0 (root of broadcast) and node 1 (root of multicast)
  if( rank < 2 ) {
    ifstream inputFile( INPUT_PATH, ios::in | ios::binary | ios::ate );
    if ( !inputFile.is_open() ) {
      cout << "Cannot open input file.\n";
      assert( false );
    }
  
    unsigned long long int fileSize = inputFile.tellg();
    unsigned long long int numLine = fileSize / LINE_SIZE;
    inputFile.seekg( 0 );

    for( unsigned long long i = 0; i < numLine; i++ ) {
      unsigned char* buff = new unsigned char[ LINE_SIZE ];
      inputFile.read( ( char* ) buff, LINE_SIZE );
      lineList.push_back( buff );

      if( rank == 0 ) {
	unsigned char* mbuff = new unsigned char[ LINE_SIZE * numNode ];
	for( int i = 0; i < numNode; i++ ) {
	  memcpy( mbuff + i*LINE_SIZE, buff, LINE_SIZE );
	}
	mlineList.push_back( mbuff );
      }
      else {
	// Skip multicast implementation for now
      }
    }
    inputFile.close();
  }


  // Broadcast measurement
  if( rank == 0 ) {
    bcComm.Barrier();
    time = clock();
    testBroadcastRoot( numNode, lineList, bcComm );
    bcComm.Barrier();  
    time = clock() - time;
    cout << rank << ": Broadcast time is " << double( time ) / CLOCKS_PER_SEC << " seconds.\n";
  }
  else {
    bcComm.Barrier();
    testBroadcast( rank, bcComm );
    bcComm.Barrier();
  }

  
  // Unicast measurement
  if( rank == 0 ) {
    bcComm.Barrier();
    time = clock();
    testUnicastRoot( numNode, lineList, bcComm );
    bcComm.Barrier();
    time = clock() - time;
    cout << rank << ": Unicast time is " << double( time ) / CLOCKS_PER_SEC << " seconds.\n";  
  }
  else {
    bcComm.Barrier();
    testUnicast( rank, bcComm );
    bcComm.Barrier();
  }

  // Scatter measurement
  if( rank == 0 ) {
    bcComm.Barrier();
    time = clock();
    testScatterRoot( numNode, mlineList, bcComm );
    bcComm.Barrier();  
    time = clock() - time;
    cout << rank << ": Scatter time is " << double( time ) / CLOCKS_PER_SEC << " seconds.\n";
  }
  else {
    bcComm.Barrier();
    testScatter( rank, bcComm );
    bcComm.Barrier();
  }


  // // Multicast
  // MPI::Intracomm mcComm = bcComm.Split( ( rank > 0 ) , rank );
  // int newRank = mcComm.Get_rank();
  // int newNumNode = mcComm.Get_size();
  // if( rank > 0 ) {
  //   if( newRank == 0 ) {
  //     //cout << rank << ": wait for barrier\n";      
  //     mcComm.Barrier();
  //     //cout << rank << ": start sending\n";      
  //     time = clock();
  //     testBroadcastRoot( newNumNode, lineList, mcComm );
  //     mcComm.Barrier();  
  //     time = clock() - time;
  //     cout << rank << ": Multicast time is " << double( time ) / CLOCKS_PER_SEC << " seconds.\n";
  //   }
  //   else {
  //     //cout << rank << ": wait for barrier\n";
  //     mcComm.Barrier();
  //     testBroadcast( newRank, mcComm );
  //     //cout << rank << ": received data\n";      
  //     mcComm.Barrier();
  //   }
  // }
    
  // Clean up
  if( rank == 0 ) {
    for( auto lit = lineList.begin(); lit != lineList.end(); lit++ ) {
      delete [] *lit;
    }
  }

  MPI::Finalize();

  return 0;
}

void testBroadcastRoot( int numNode, LineList& lineList, MPI::Intracomm& comm )
{
  unsigned long long numLine = lineList.size();
  comm.Bcast( &numLine, 1, MPI::UNSIGNED_LONG_LONG, 0 );
  for( auto it = lineList.begin(); it != lineList.end(); it++ ) {
    comm.Bcast( ( char* ) *it, LINE_SIZE, MPI::UNSIGNED_CHAR, 0 );      
  }
}

void testBroadcast( int rank, MPI::Intracomm& comm )
{
  unsigned long long numLine;
  comm.Bcast( &numLine, 1, MPI::UNSIGNED_LONG_LONG, 0 );
  for( unsigned long long i = 0; i < numLine; i++ ) {
    unsigned char t[ LINE_SIZE ];
    comm.Bcast( ( char* ) t, LINE_SIZE, MPI::UNSIGNED_CHAR, 0 );
  }
}

void testUnicastRoot( int numNode, LineList& lineList, MPI::Intracomm& comm )
{
  unsigned long long numLine = lineList.size();
  for( int nid = 1; nid < numNode; nid++ ) {
    comm.Send( &numLine, 1, MPI::UNSIGNED_LONG_LONG, nid, 0 );
    for( auto it = lineList.begin(); it != lineList.end(); it++ ) {
      comm.Send( ( char* ) *it, LINE_SIZE, MPI::UNSIGNED_CHAR, nid, 0 );      
    }  
  }
}

void testUnicast( int rank, MPI::Intracomm& comm )
{
  unsigned long long numLine;
  comm.Recv( &numLine, 1, MPI::UNSIGNED_LONG_LONG, 0, 0 );
  for( unsigned long long i = 0; i < numLine; i++ ) {
    unsigned char t[ LINE_SIZE ];
    comm.Recv( ( char* ) t, LINE_SIZE, MPI::UNSIGNED_CHAR, 0, 0 );
  }  
}

void testScatterRoot( int numNode, LineList& mlineList, MPI::Intracomm& comm )
{
  unsigned long long numLine[ numNode ];
  for( int i = 0; i < numNode; i++ ) {
    numLine[i] = mlineList.size();
  }
  unsigned long long nl;

  comm.Scatter( numLine, 1, MPI::UNSIGNED_LONG_LONG, &nl, 1, MPI::UNSIGNED_LONG_LONG, 0 );
  for( auto it = mlineList.begin(); it != mlineList.end(); it++ ) {
    unsigned char t[ LINE_SIZE ];
    comm.Scatter( *it, LINE_SIZE, MPI::UNSIGNED_CHAR, t, LINE_SIZE, MPI::UNSIGNED_CHAR, 0 );
  }
}

void testScatter( int rank, MPI::Intracomm& comm )
{
  unsigned long long numLine;
  comm.Scatter( NULL, 0, MPI::UNSIGNED_LONG_LONG, &numLine, 1, MPI::UNSIGNED_LONG_LONG, 0 );
  for( unsigned long long i = 0; i < numLine; i++ ) {
    unsigned char t[ LINE_SIZE ];
    comm.Scatter( NULL, 0, MPI::UNSIGNED_CHAR, t, LINE_SIZE, MPI::UNSIGNED_CHAR, 0 );
  }
}









