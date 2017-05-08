#ifndef _MR_TRIE
#define _MR_TRIE

#include "Common.h"
#include "Utility.h"

class TrieNode {
 private:
  int level;
  
 public:
  TrieNode( int _level ): level( _level ) {}
  virtual ~TrieNode() {}
  virtual int findPartition( unsigned char* key ) = 0;
  int getLevel() { return level; }
};


class InnerTrieNode: public TrieNode {
 public:
  TrieNode** child;

 public:
 InnerTrieNode( int _level ): TrieNode( _level ) {
    child = new TrieNode*[ 256 ];
  }
  ~InnerTrieNode() {
    for( int i = 0; i < 256; i++ ) {
      delete child[ i ];
    }
    delete [] child;
  }

  int findPartition( unsigned char* key );
  void setChild( int index, TrieNode* _child ) { child[ index ] = _child; }
};

class LeafTrieNode: public TrieNode {
 private:
  int lower;
  int upper;
  PartitionList* partitionList;
  
 public:
 LeafTrieNode( int _level, PartitionList* _partitionList, int _lower, int _upper ): TrieNode( _level ), lower( _lower ), upper( _upper ), partitionList( _partitionList ) {}
  ~LeafTrieNode() {}
  int findPartition( unsigned char* key );
};


class Sorter {
 private:
  unsigned int length;
 public:
 Sorter( unsigned int _length ): length( _length ) {}
  bool operator()( const unsigned char* keyl, const unsigned char* keyr ) { return cmpKey( keyl, keyr, length ); }
};


#endif
