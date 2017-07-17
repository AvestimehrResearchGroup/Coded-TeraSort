#ifndef _MR_WORKER
#define _MR_WORKER

#include <unordered_map>

#include "Configuration.h"
#include "Common.h"
#include "Utility.h"
#include "Trie.h"

class Worker
{
 public:
  typedef unordered_map< unsigned int, LineList* > PartitionCollection;
  typedef struct _TxData {
    unsigned char* data;
    unsigned long long numLine; // Number of lines
  } TxData;
  typedef unordered_map< unsigned int, TxData > PartitionPackData;  // key in { 0, 1, 2, ... }

 private:
  const Configuration* conf;
  unsigned int rank;
  PartitionList partitionList;
  PartitionCollection partitionCollection;
  PartitionPackData partitionTxData;
  PartitionPackData partitionRxData;
  LineList localList;
  TrieNode* trie;
  clock_t memTime;

 public:
 Worker( unsigned int _rank ): rank( _rank ) {}
  ~Worker();
  void run();

 private:
  void execMap();
  void execReduce();
  void printLocalList();
  void printPartitionCollection();
  void outputLocalList();
  TrieNode* buildTrie( PartitionList* partitionList, int lower, int upper, unsigned char* prefix, int prefixSize, int maxDepth );
};


#endif
