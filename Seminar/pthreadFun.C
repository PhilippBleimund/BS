/* ===========================================================================
 *
 * pthreadFun.C --
 *
 *
 * Author: Wolfram Schenck <wolfram.schenck@fh-bielefeld.de>
 *
 *
 * 1.0 / 26. Nov 15 (ws)
 * - from scratch
 *
 *
 * ===========================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


// ---------------------------------------------------------------------------
// GLOBAL VARIABLES AND CONSTANTS
// ---------------------------------------------------------------------------

// matrix size (both width and height)
#define MATRIX_SIZE 4

// matrix A
double A[MATRIX_SIZE][MATRIX_SIZE] =
                 {{1.0,2.0,3.0,4.0},
                  {11.0,12.0,13.0,14.0},
                  {21.0,22.0,23.0,24.0},
                  {31.0,32.0,33.0,34.0}};

// matrix B
double B[MATRIX_SIZE][MATRIX_SIZE] =
                 {{31.0,32.0,33.0,34.0},
                  {21.0,22.0,23.0,24.0},
                  {11.0,12.0,13.0,14.0},
                  {1.0,2.0,3.0,4.0}};

// matrix C
double C[MATRIX_SIZE][MATRIX_SIZE];


// ---------------------------------------------------------------------------
// DATA TYPES
// ---------------------------------------------------------------------------

// struct to collect all information necessary for the
// specification of a (partial) matrix multiplication
typedef struct {
  double (*C)[MATRIX_SIZE]; // result matrix: C = A*B
  double (*A)[MATRIX_SIZE]; // matrix A (see above)
  double (*B)[MATRIX_SIZE]; // matrix B (see above)
  unsigned firstRow;  // index of first row of C (incl.)
  unsigned lastRow;   // index of last row of C (incl.)
} multArg;


// ---------------------------------------------------------------------------
// FUNCTIONS
// ---------------------------------------------------------------------------

// helper function to print matrix M to stdout
void
printMatrix( const double (*M)[MATRIX_SIZE] )
{
  for( unsigned row = 0; row < MATRIX_SIZE; row++ )
  {
    for( unsigned col = 0; col < MATRIX_SIZE; col++ )
    {
      printf( "%8.2lf ", M[row][col] );
    }
    printf( "\n" );
  }

  return;
}


// function for matrix multiplication;
// only the rows firstRow to lastRow (defined in MA) are computed
void
multMatrix( multArg MA )
{
  // iterate over rows of C (only from firstRow to lastRow!)
  for( unsigned row = MA.firstRow; row <= MA.lastRow; row++ )
  {
    // iterate over columns of C
    for( unsigned col = 0; col < MATRIX_SIZE; col++ )
    {
      MA.C[row][col] = 0.0;
      // iterate over inner dimension of A and B
      for( unsigned ind = 0; ind < MATRIX_SIZE; ind++ )
      {
        MA.C[row][col] += MA.A[row][ind] * MA.B[ind][col];
      }
    }
  }

  return;
}


// thread startup function; wrapper function for multMatrix(..)
void*
multMatrixThreadWrapper( void* arg )
{
  // type conversion for argument to multMatrix
  // (from void* to multArg*)
  multArg *MA = (multArg*)arg;
  // call multMatrix
  multMatrix( *MA );

  // artificial halt to be able to observe number of threads with
  // watch 'ps -eLf | grep pthreadFun'
  // or
  // watch 'ps -eLo pid,lwp,comm | grep pthreadFun'
  // usleep( 1e9 );

  return NULL;
}


// ---------------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------------

int
main()
{
  // define matrix multiplication operations
  multArg MA_0 = {C, A, B, 0, 0};
  multArg MA_1 = {C, A, B, 1, 1};
  multArg MA_2 = {C, A, B, 2, 2}; 
  multArg MA_3 = {C, A, B, 3, 3}; 

  // thread id
  pthread_t tid[3];

  // create thread for partial matrix multiplication (for rows 2-3)
  pthread_create(&tid[0], NULL, multMatrixThreadWrapper, &MA_1 );
  pthread_create(&tid[1], NULL, multMatrixThreadWrapper, &MA_2 );
  pthread_create(&tid[2], NULL, multMatrixThreadWrapper, &MA_3 );

  // carry out partial multiplication (for rows 0-1)
  multMatrix( MA_0 );

  // artificial halt to be able to observe number of threads with
  // watch 'ps -eLf | grep pthreadFun'
  // or
  // watch 'ps -eLo pid,lwp,comm | grep pthreadFun'
  // usleep( 1e9 );

  // join thread which was created above
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);

  // print out all matrices
  printf( "A = \n" );
  printMatrix( A );
  printf( "\nB =\n" );
  printMatrix( B );
  printf( "\nC =\n" );
  printMatrix( C );

  exit(0);
}


// ---------------------------------------------------------------------------
// Command to build program from console:
// g++ pthreadFun.C -o pthreadFun -lpthread
// ---------------------------------------------------------------------------
