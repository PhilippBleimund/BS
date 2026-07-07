/* ===========================================================================
 *
 * memfit.C --
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
#include <time.h>

#define MEM_MIN_ADDRESS  100

unsigned ALLOC_MAX; 
unsigned MEM_SIZE;

// ---------------------------------------------------------------------------

enum allocAlg_t {
  FIRST_FIT, NEXT_FIT, BEST_FIT, WORST_FIT
};

// ---------------------------------------------------------------------------

typedef struct {
  unsigned count;
  double elapsedSeconds;  // total time spent inside the alloc algorithm calls
} SimResult;

SimResult
simulateAllocs( unsigned seedVal, allocAlg_t allocAlg )
{
  SimResult result = {0, 0.0};

  initHeap( MEM_MIN_ADDRESS, MEM_SIZE );
  srand(seedVal);

  struct timespec tStart, tEnd;

  do {
    if( ((rand() % 2) == 1) && (allocList.size() > 0) )
    {
      randomFree();
    }
    else
    {
      unsigned address;
      unsigned memRequest = (rand() % ALLOC_MAX) + 1;

      clock_gettime(CLOCK_MONOTONIC, &tStart);
      switch( allocAlg )
      {
        case FIRST_FIT:
          address = firstFitAlloc( memRequest );
          break;
        case NEXT_FIT:
          address = nextFitAlloc( memRequest );
          break;
        case BEST_FIT:
          address = bestFitAlloc( memRequest );
          break;
        case WORST_FIT:
        default:
          address = worstFitAlloc( memRequest );
          break;
      }
      clock_gettime(CLOCK_MONOTONIC, &tEnd);

      result.elapsedSeconds += (tEnd.tv_sec - tStart.tv_sec)
                              + (tEnd.tv_nsec - tStart.tv_nsec) / 1e9;

      if( !address )
        break;
    }

    result.count++;
  } while(1);

  return result;
}

// ---------------------------------------------------------------------------

int
main( int argc, char **argv )
{
  // parse input argument
  if (argc > 2){
    ALLOC_MAX = strtol(argv[1], NULL, 0);
    MEM_SIZE = strtol(argv[2], NULL, 0);
  }else{
    ALLOC_MAX = 100;
    MEM_SIZE = 1000;
  }

  // use array count to store results for the four different mem. alloc. methods
  unsigned count[4] = {0,0,0,0};
  double   elapsed[4] = {0,0,0,0};

  allocAlg_t algs[4] = {FIRST_FIT, NEXT_FIT, BEST_FIT, WORST_FIT};

  unsigned itCount = 0;
  for( unsigned seedVal = 0; seedVal < 100000; seedVal += 1000 )
  {
    for (int a = 0; a < 4; a++) {
      SimResult r = simulateAllocs( seedVal, algs[a] );
      count[a] += r.count;
      elapsed[a] += r.elapsedSeconds;
    }
    itCount++;
  }

  // print results to console
  printf( "SUMMARY\n=======\n\n" );
  printf( "FIRST_FIT_COUNT: %u\n", count[0] );
  printf( "NEXT_FIT_COUNT: %u\n", count[1] );
  printf( "BEST_FIT_COUNT: %u\n", count[2] );
  printf( "WORST_FIT_COUNT: %u\n", count[3] );
  printf( "FIRST_FIT_TIME: %.9f\n", elapsed[0] );
  printf( "NEXT_FIT_TIME: %.9f\n", elapsed[1] );
  printf( "BEST_FIT_TIME: %.9f\n", elapsed[2] );
  printf( "WORST_FIT_TIME: %.9f\n", elapsed[3] );

  printf( "\nTrials   : %u\n", itCount );

  return 0;
}
