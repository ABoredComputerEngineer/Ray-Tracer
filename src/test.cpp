#include <stdio.h>
#include <inttypes.h>
#define OS_LINUX_CPP
#include "../include/prng.h"

int main( void ){
  prng_seed();
  for ( int i = 0; i < 32; i++ ){
    fprintf( stdout, "%f\n", prng_float() );
  }
  return 0;
}
