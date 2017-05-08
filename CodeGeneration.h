#ifndef _CMR_CODEGENERATION
#define _CMR_CODEGENERATION

#include <vector>
#include <set>
#include <map>
#include <utility>
#include <unordered_map>
#include <assert.h>

using namespace std;

typedef set< int > NodeSet;
typedef set< int > InputSet;
typedef vector< vector< bool > > ImMatrix;
typedef pair< int, int > Vpair; // < destId, inputId >
typedef vector< Vpair > VpairList;
typedef struct _Vj {
  VpairList vpList;
  int dest;
  int order;
_Vj( VpairList vpl, int _dest, int _order ): vpList( vpl ), dest( _dest ), order( _order ) {}
} Vj;
typedef vector< Vj > VjList;
typedef unsigned int SubsetSId;

class CodeGeneration {
 private:
  int N;
  int K;
  int R;
  int Eta;
  vector< NodeSet > NodeSubsetR; // subset of nodes of size R
  vector< NodeSet > NodeSubsetS; // subset of nodes of size R+1
  map< NodeSet, SubsetSId > SubsetSIdMap;  
  /* unordered_map< int, InputSet > M;  // Map: key = nodeId, value = files at the node */
  /* unordered_map< int, ImMatrix > NodeImMatrix; */
  map< int, InputSet > M;  // Map: key = nodeId, value = files at the node
  map< int, ImMatrix > NodeImMatrix;  

  /* unordered_map< SubsetSId, unordered_map< int, VpairList > > SubsetDestVpairList; */
  /* unordered_map< SubsetSId, unordered_map< int, VjList > > SubsetSrcVjList; */

  /* unordered_map< int, vector< NodeSet > > NodeSubsetSMap; // Map: key = nodeID, value = list of subset of size R+1 containing nodeId */
  /* unordered_map< unsigned long, NodeSet > FileNodeMap; */
  map< int, vector< NodeSet > > NodeSubsetSMap; // Map: key = nodeID, value = list of subset of size R+1 containing nodeId
  map< unsigned long, NodeSet > FileNodeMap;  
  map< NodeSet, unsigned long > NodeFileMap;

 public:
  CodeGeneration( int _N, int _K, int _R );
  ~CodeGeneration() {}
  static void printNodeSet( NodeSet ns );
  static void printVpairList( VpairList vpl );

  vector< NodeSet >& getNodeSubsetR() { return NodeSubsetR; }
  vector< NodeSet >& getNodeSubsetS() { return NodeSubsetS; }
  map< NodeSet, SubsetSId >& getSubsetSIdMap() { return SubsetSIdMap; }
  /* unordered_map< int, InputSet >& getM() { return M; } */
  map< int, InputSet >& getM() { return M; }  
  InputSet& getM( int nodeId ) { return M[ nodeId ]; }
  /* unordered_map< int, ImMatrix >& getNodeImMatrix() { return NodeImMatrix; } */
  map< int, ImMatrix >& getNodeImMatrix() { return NodeImMatrix; }
  
  /* unordered_map< SubsetSId, unordered_map< int, VpairList > >& getSubsetDestVpairList() { return SubsetDestVpairList; } */
  /* unordered_map< SubsetSId, unordered_map< int, VjList > >& getSubsetSrcVjList() { return SubsetSrcVjList; } */
  
  int getEta() { return Eta; }
  int getN() { return N; }
  int getK() { return K; }
  int getR() { return R; }
  SubsetSId getSubsetSId( NodeSet ns );
  NodeSet getSubsetSFromId( SubsetSId id ) { return NodeSubsetS[ id ]; }
  vector< NodeSet >& getNodeSubsetSContain( int nid ) { return NodeSubsetSMap[ nid ]; }
  NodeSet& getNodeSetFromFileID( unsigned long fid ) { return FileNodeMap[ fid ]; }
  unsigned long getFileIDFromNodeSet( NodeSet ns ) { return NodeFileMap[ ns ]; }

 private:
  vector< NodeSet > generateNodeSubset( int r );
  void generateSubset( NodeSet preset, NodeSet remain, unsigned int size, vector< NodeSet >& list );
  
  void constructM();
  ImMatrix generateImMatrix( int nid );
  void generateSubsetDestVpairList();
  void generateSubsetSrcVjList();

  vector< NodeSet > generateNodeSubsetContain( int nodeId, int r );
};


#endif
