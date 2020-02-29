#include <stdio.h>
#include <inttypes.h>
#define OS_LINUX_CPP
#define HANDMADE_MATH_IMPLEMENTATION
#include <float.h>
#include "common.h"
#include "../include/prng.h"
#include "../include/HandmadeMath.h"
#include "../include/primitives.h"
#include <cstring>

int less_than( void *x ){
  return *(int *)x < 10;
}

void print_m4( m4 &m ){
  for ( int i = 0; i < 4; i++ ){
    for (int j = 0; j < 4; j++ ){
      fprintf( stdout, "%f ", m[i][j]);
    }
    fprintf( stdout, "\n" );
  }
}

int main( void ){
  m4 projection = HMM_Perspective(40,
                  800.0f/600.0f,
                  0.1f, 10.0f );
  m4 view = HMM_LookAt( 
            v3{ 0.0f, 0.5f, 5.0f },
            v3{ 0.0f, 0.5f, -1.0f },
            v3{ 0.0f, 1.0f, 0.0f } );
  m4 mv = projection *view;
  print_m4( mv );
  fprintf( stdout, "\nThe inverse is: \n" );
  m4 inv = GetInverse( projection );
  print_m4( inv );
  v3 point = { 400, 400, 5.0f };
  
  uint x = UINT_MAX;
  uint y[2] = { x + 1, 23 };
  uint *z = y;
  fprintf( stdout, "y = %u\n", *z++ );
  fprintf( stdout, "y = %u\n", *z );
}
