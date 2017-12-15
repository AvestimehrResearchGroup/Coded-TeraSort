
#include <assert.h>
#include "Trie.h"

int InnerTrieNode::findPartition( unsigned char* key )
{
  int level = getLevel();
  assert( level < 10 );
  return child[ key[ level & 0xFF ] ]->findPartition( key );
}


int LeafTrieNode::findPartition( unsigned char* key )
{
  for( int i = lower; i < upper; i++ ) {
    if ( cmpKey( key, partitionList->at( i ), 10 ) ) {
      return i;
    }
  }
  return upper;
}
