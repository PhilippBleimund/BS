/* ===========================================================================
 *
 * memfitHelpers.H --
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

#ifndef __MEM_FIT_HELPERS_H__
#define __MEM_FIT_HELPERS_H__

#include <stdio.h>
#include <stdlib.h>
#include <vector>

// ---------------------------------------------------------------------------

#define VERBOSE  0

// ---------------------------------------------------------------------------

struct memBlock {
  unsigned address;
  unsigned size;
};

extern std::vector<memBlock> freeList;
extern std::vector<memBlock> allocList;

// ---------------------------------------------------------------------------

void
initHeap( unsigned minAddress, unsigned heapSize );

void
printList( const char *header, const std::vector<memBlock> blockList );

void
randomFree();

unsigned
allocBlock( unsigned index, unsigned blockSize );

unsigned
meldFreeBlocks();

int
freeBlock( unsigned address );

// ---------------------------------------------------------------------------

unsigned
firstFitAlloc( unsigned blockSize );

unsigned
nextFitAlloc( unsigned blockSize );

unsigned
bestFitAlloc( unsigned blockSize );

unsigned
worstFitAlloc( unsigned blockSize );


#endif // __MEM_FIT_HELPERS_H__

