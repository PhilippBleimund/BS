/* ===========================================================================
 *
 * memfitHelpers.C --
 *
 *
 * Wolfram Schenck <wolfram.schenck@fh-bielefeld.de>
 *
 *
 * 1.0 / 17. Nov 15 (ws)
 * - from scratch
 *
 *
 * ===========================================================================
 */

#include "memfitHelpers.H"

// ---------------------------------------------------------------------------

// list of free blocks (sorted by start address)
std::vector<memBlock> freeList;
// list of allocated blocks (sorted by start address)
std::vector<memBlock> allocList;

// ---------------------------------------------------------------------------

void
initHeap( unsigned minAddress, unsigned heapSize )
{
  // prevent addresses from starting at zero because
  // zero is reserved as error value (NULL)
  if( minAddress == 0 )
    minAddress++;

  // no allocated mem. in the beginning
  allocList.resize(0);
  // one single block of free mem. in the beginning
  freeList.resize(1);
  freeList[0].address = minAddress;
  freeList[0].size = heapSize;
}


void
printList( const char *header, const std::vector<memBlock> blockList )
{
  printf( "%s\n--------------------\n", header );

  for( unsigned u=0; u < blockList.size(); u++ )
  {
    printf( "%8u  len: %5u\n", blockList[u].address, blockList[u].size );
  }

  printf( "\n" );

  return;
}


void randomFree()
{
  unsigned max = allocList.size();
  if( max > 0 ) {
    // random index (within allocList)
    unsigned index = rand() % max;
    // get address of this block
    unsigned address = allocList[index].address;
    // free this block
    int res = freeBlock( address );
    if( VERBOSE )
    {
      if( res == 0 )
      {
        printf("RANDOMFREE: Success in freeing block no. %u at address %u!\n\n",
              index, address);
      }
      else
      {
        printf("RANDOMFREE: Failure to free block no. %u at address %u!\n\n",
              index, address);
      }
    }
  }
  return;
}


unsigned
allocBlock( unsigned index, unsigned blockSize )
{
  if( index >= freeList.size() )
    return 0; // ERROR

  if( freeList[index].size < blockSize )
    return 0; // ERROR

  // set attributes of newly allocated block
  memBlock newAllocBlock;
  newAllocBlock.size = blockSize;
  // the space is allocated at the beginning
  // of the free block
  newAllocBlock.address = freeList[index].address;

  // selected block in freeList has exactly the right size?
  // ==> just delete the block from the freeList
  if( freeList[index].size == blockSize )
  {
    freeList.erase( freeList.begin() + index );
  }
  else
  {
    // take allocated mem. from beginning of the block in freeList
    freeList[index].size -= blockSize;
    freeList[index].address += blockSize;
  }

  // allocList is empty?
  // ==> just append newly allocated block
  if( allocList.size() == 0 )
  {
    allocList.push_back( newAllocBlock );
    return newAllocBlock.address;
  }

  // continue here if allocList.size() > 0
  // find right location in allocList to insert newly allocated block
  // (allocList sorted by start addresses)
  unsigned insertPos;
  for( insertPos=0; insertPos < allocList.size(); insertPos++ )
  {
    if( allocList[insertPos].address > newAllocBlock.address )
    {
      break;
    }
  }
  // insert newly allocated block
  allocList.insert( allocList.begin() + insertPos, newAllocBlock );

  // return start address of newly allocated block
  return newAllocBlock.address;
}


unsigned
meldFreeBlocks()
{
  unsigned meldCount = 0;

  // iterate over all blocks in freeList
  for( int i=0; i < ((int)freeList.size()-1); i++ )
  {
     // check if there is a gap between blocks i and i+1
     // if not, meld blocks
     if( (freeList[i].address + freeList[i].size) == freeList[i+1].address )
     {
       freeList[i].size += freeList[i+1].size;
       freeList.erase( freeList.begin() + (i+1) );
       i--; // necessary to start again at block which was just melded
       meldCount++;
     }
  }

  return meldCount;
}


int
freeBlock( unsigned address )
{
  if( allocList.size() == 0 )
    return -1; // ERROR

  // search for index of block with specified start address
  // within allocList
  unsigned int freeIndex = allocList.size();
  for( unsigned u=0; u < allocList.size(); u++ )
  {
    if( allocList[u].address == address )
    {
      freeIndex = u;
      break;
    }
  }

  // not found?
  // ==> return with error value
  if( freeIndex == allocList.size() )
    return -1; // ERROR

  // set attributes of new free block
  memBlock newFreeBlock;
  newFreeBlock.address = address;
  newFreeBlock.size = allocList[freeIndex].size;

  // erase previously allocated block from allocList
  allocList.erase( allocList.begin() + freeIndex );

  // freeList is empty?
  // ==> just append new free block
  if( freeList.size() == 0 )
  {
    freeList.push_back( newFreeBlock );
    if( VERBOSE )
      printf( "FREE: Empty free list\n\n" );
    return 0;
  }

  // continue here if freeList.size() > 0
  // find right location in freeList to insert new free block
  // (freeList sorted by start addresses)
  unsigned insertPos;
  for( insertPos=0; insertPos < freeList.size(); insertPos++ )
  {
    if( freeList[insertPos].address > newFreeBlock.address )
    {
      break;
    }
  }
  // insert new free block
  freeList.insert( freeList.begin() + insertPos, newFreeBlock );

  // meld blocks in freeList
  unsigned meldCount = meldFreeBlocks();
  if( VERBOSE )
    printf( "FREE: Non-empty free list; meldCount = %i\n\n", meldCount );

  return 0;
}


