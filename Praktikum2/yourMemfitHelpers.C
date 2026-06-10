/* ===========================================================================
 *
 * yourMemfitHelpers.C --
 *
 *
 * 1.0 / 17. Nov 15 (ws)
 * - from scratch
 *
 *
 * ===========================================================================
 */

#include "memfitHelpers.H"


unsigned
firstFitAlloc( unsigned blockSize )
{  
  unsigned address = 0;
  
  // loop over free space to check if there is a block bigger than blockSize
  for (unsigned i=0;i<freeList.size();i++){
    if (freeList[i].size >= blockSize){
      // free space found 
      address = allocBlock(i, blockSize);
    }
  }
  
  return address;
}

volatile unsigned lastAddressNextFit = 0;

unsigned
nextFitAlloc( unsigned blockSize )
{
  unsigned address = 0;

  if (lastAddressNextFit == 0){
    lastAddressNextFit = freeList.front().address;
  }

  // search for lastAddressNextFit
  unsigned lastAddress_idx = 0;
  for (unsigned i=0;i<freeList.size();i++){
    if (freeList[i].address == lastAddressNextFit){
      lastAddress_idx = i;
      break;
    }
  }

  // loop over free space to check if there is a block bigger than blockSize
  // start at last Address
  int found = 0;
  for (unsigned i=lastAddress_idx;i<freeList.size();i++){
    if (freeList[i].size >= blockSize){
      // free space found
      found = 1; 
      address = allocBlock(i, blockSize);
    }
  }
  // if nothing was found start from first address
  if (!found){
    for (unsigned i=0;i<lastAddress_idx;i++){
      if (freeList[i].size >= blockSize){
        // free space found
        address = allocBlock(i, blockSize);
      }
    }
  }
  lastAddressNextFit = address;

  return address;
}


unsigned
bestFitAlloc( unsigned blockSize )
{
  unsigned address = 0;

  // loop over free space to find smallest fitting block
  unsigned best = freeList.size()-1;
  unsigned lastFit = blockSize;
  for (unsigned i=0;i<freeList.size();i++){
    int nextFit = freeList[i].size - blockSize;
    if (nextFit >= 0 && (unsigned) nextFit < lastFit){
      best = i;
      lastFit = nextFit;
    }
  }
  address = allocBlock(best, blockSize);

  return address;
}


unsigned
worstFitAlloc( unsigned blockSize )
{
  unsigned address = 0;

  // loop over free space to find biggest fitting block
  unsigned best = freeList.size()-1;
  unsigned lastFit = 0;
  for (unsigned i=0;i<freeList.size();i++){
    int nextFit = freeList[i].size - blockSize;
    if (nextFit >= 0 && (unsigned) nextFit > lastFit){
      best = i;
      lastFit = nextFit;
    }
  }
  address = allocBlock(best, blockSize);

  return address;
}
