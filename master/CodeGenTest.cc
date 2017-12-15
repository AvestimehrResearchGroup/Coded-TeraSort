#include <iostream>

#include "CodeGeneration.h"

using namespace std;


int main( int argc, char* argv[] ) {
  if ( argc < 4 ) {
    cout << "Input format: N K R\n";
    return 0;
  }

  CodeGeneration cg( atoi( argv[1] ), atoi( argv[2] ), atoi( argv[3] ) );
  
  vector< NodeSet > nR = cg.getNodeSubsetR();
  cout << "Subset of nodes of size " << cg.getR() << ":\n";
  for ( auto it = nR.begin(); it != nR.end(); ++it ) {
    CodeGeneration::printNodeSet( *it );
    cout << endl;
  }

  int eta = cg.getEta();
  cout << endl;  
  cout << "Eta = " << eta << endl;

  unordered_map< int, InputSet > m = cg.getM();
  cout << endl;
  cout << "Set M:\n";
  for ( auto it = m.begin(); it != m.end(); ++it ) {
    int nid = it->first;
    InputSet inputs = it->second;
    cout << "Node " << nid << " processes inputs: ";
    for ( auto it2 = inputs.begin(); it2 != inputs.end(); ++it2 ) {
      cout << *it2 << ' ';
    }
    cout << endl;
  }

  unordered_map< int, ImMatrix > nim = cg.getNodeImMatrix();
  cout << endl;
  cout << "Intermediate values ( q, n ):\n";
  for( int k = 1; k <= cg.getK(); k++ ) {
    cout << "Node " << k << endl;
    ImMatrix im = nim[ k ];
    for( int i = 0; i < cg.getK(); i++ ) {
      for( int j = 0; j < cg.getN(); j++ ) {
	cout << int( im[ i ][ j ] ) << ' ';
      }
      cout << endl;
    }
    cout << endl;
  }

  vector< NodeSet > nS = cg.getNodeSubsetS();
  cout << endl;
  cout << "Subset of nodes of size " << cg.getR() + 1 << ":\n";
  for ( auto it = nS.begin(); it != nS.end(); ++it ) {
    CodeGeneration::printNodeSet( *it );
    cout << endl;
  }

  unordered_map< SubsetSId, unordered_map< int, VpairList > > sdstvlist = cg.getSubsetDestVpairList();
  cout << endl;
  cout << "Exclusive intermediate values:\n";
  map< NodeSet, SubsetSId > sidmap = cg.getSubsetSIdMap();
  for( auto sit = sidmap.begin(); sit != sidmap.end(); sit++ ) {
    NodeSet s = sit->first;
    SubsetSId id = sit->second;
    cout << "Subset ";
    CodeGeneration::printNodeSet( s );
    cout << endl;
    for( auto qit = s.begin(); qit != s.end(); ++qit ) {
      int q = *qit;
      cout << "  Node " << q << " needs: ";
      VpairList l = sdstvlist[ id ][ q ];
      for( unsigned int i = 0; i < l.size(); i++ ) {
	cout << "( " << l[i].first << ", " << l[i].second << " ),  ";
      }
      NodeSet t = s;
      t.erase( q );
      cout << "// owned by nodes in ";
      CodeGeneration::printNodeSet( t );
      cout << endl;
    }
  }

  unordered_map< SubsetSId, unordered_map< int, VjList > > ssrcvlist = cg.getSubsetSrcVjList();
  cout << endl;
  cout << "Construct data for encoding:\n";
  for( auto sit = sidmap.begin(); sit != sidmap.end(); sit++ ) {
    NodeSet s = sit->first;
    SubsetSId id = sit->second;
    cout << "Subset ";
    CodeGeneration::printNodeSet( s );
    cout << endl;
    for( auto srcit = ssrcvlist[ id ].begin(); srcit != ssrcvlist[ id ].end(); srcit++ ) {
      int src = srcit->first;
      VjList dataList = srcit->second;
      cout << "  Node " << src << ":\n";
      for( unsigned int i = 0; i < dataList.size(); i++ ) {
	Vj &vj = dataList[ i ];
	cout << "    From data ";
	CodeGeneration::printVpairList( vj.vpList );
	cout << ", the " << vj.order << "th path is used for encoding.\n";
      }
    }
  }

  cout << endl;
  cout << "Multicast scheduling:\n";
  for( int k = 1; k <= cg.getK(); k++ ) {
    cout << "Node " << k << " multicast:\n";
    for( auto sit = nS.begin(); sit != nS.end(); sit++ ) {
      NodeSet s = *sit;
      if( s.find( k ) != s.end() ) {
	cout << "  Subset ";
	CodeGeneration::printNodeSet( s );
	cout << endl;
      }
    }
  }

  cout << endl;
  cout << "Subset contain:\n";
  for( int i = 1; i <= cg.getK(); i++ ) {
    cout << "  i = " << i << endl;
    vector< NodeSet > ns = cg.getNodeSubsetSContain( i );
    for( auto it = ns.begin(); it != ns.end(); it++ ) {
      CodeGeneration::printNodeSet( *it );
      cout << endl;
    }
  }

  cout << endl;
  cout << "FileId to a set of nodes:\n";
  for( unsigned long fid = 1; fid <= (unsigned int ) cg.getN(); fid++ ) {
    cout << "  fid = " << fid << " : ";
    NodeSet ns = cg.getNodeSetFromFileID( fid );
    CodeGeneration::printNodeSet( ns );
    cout << endl;
  }
  
  return 0;
}

