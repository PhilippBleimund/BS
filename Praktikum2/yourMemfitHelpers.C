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
#include <limits.h>

unsigned firstFitAlloc(unsigned blockSize) {
  unsigned address = 0;

  // loop over free space to check if there is a block bigger than blockSize
  for (unsigned i = 0; i < freeList.size(); i++) {
    if (freeList[i].size >= blockSize) {
      // free space found
      address = allocBlock(i, blockSize);
      break;
    }
  }

  return address;
}

unsigned nextFitAlloc(unsigned blockSize) {
  unsigned lastAddressNextFit = 0; 
  unsigned address = 0;

  if (freeList.empty())
    return address;

  if (lastAddressNextFit == 0) {
    lastAddressNextFit = freeList.front().address;
  }

  // search for lastAddressNextFit
  unsigned lastAddress_idx = 0;
  for (unsigned i = 0; i < freeList.size()-1; i++) {
    if (freeList[i].address >= lastAddressNextFit && lastAddressNextFit <= freeList[i+1].address) {
      lastAddress_idx = i;
      break;
    }
  }

  // loop over free space to check if there is a block bigger than blockSize
  // start at last Address
  unsigned n = freeList.size();
  for (unsigned j = 0; j < n; j++) {
    unsigned i = (lastAddress_idx + j) % n;
    if (freeList[i].size >= blockSize) {
      lastAddressNextFit = freeList[i].address;
      address = allocBlock(i, blockSize);
      break;
    }
  }
  if (address == 0)
    lastAddressNextFit = 0;

  return address;
}

unsigned bestFitAlloc(unsigned blockSize) {
  unsigned address = 0;

  // loop over free space to find smallest fitting block
  int best = -1;
  unsigned lastFit = UINT_MAX;
  for (unsigned i = 0; i < freeList.size(); i++) {
    int nextFit = freeList[i].size - blockSize;
    if (nextFit >= 0 && (unsigned)nextFit < lastFit) {
      best = i;
      lastFit = nextFit;
    }
  }
  if (best == -1)
    return 0;
  address = allocBlock(best, blockSize);

  return address;
}

unsigned worstFitAlloc(unsigned blockSize) {
  unsigned address = 0;

  // loop over free space to find biggest fitting block
  int best = -1;
  unsigned lastFit = 0;
  for (unsigned i = 0; i < freeList.size(); i++) {
    int nextFit = freeList[i].size - blockSize;
    if (nextFit >= 0 && (unsigned)nextFit > lastFit) {
      best = i;
      lastFit = nextFit;
    }
  }
  if (best == -1)
    return 0;
  address = allocBlock(best, blockSize);

  return address;
}
