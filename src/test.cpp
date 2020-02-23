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
struct Ray {
  v3 start;
  v3 direction;
  v3 inv_dir;
  int sign[3];

  inline v3 point_at( float t ) const{
    return start + t*(direction);
  }

  Ray ( v3 origin, v3 dir ):start(origin), direction( dir ) {
    sign[0] = dir.X < 0;
    sign[1] = dir.Y < 0;
    sign[2] = dir.Z < 0;

    inv_dir = v3{ 1/dir.X, 1/dir.Y, 1/dir.Z };
  }
  Ray (){}
};

bool AABB_hit( 
    const AABB &box,
    const Ray &ray,
    float t0,
    float t1 )
{
  
  // Calculate for x and y
  // For eg. for direction = { -1.0f, -1.0f ,-1.0f }
  // and bounds lower=[ -5, -5, -5 ] and  upper=[ -3, -3, -3 ]
  // ray.sign = { 1, 1,1 }
  // xnear = box.bounds[ 1 ].X = -3
  // xfar = box.bounds[ 1 - 1 ].X = -5
  float xnear = box.bounds[ ray.sign[0] ].X; 
  float xfar = box.bounds[ 1-ray.sign[0] ].X;

  float ynear = box.bounds[ ray.sign[1] ].Y; 
  float yfar = box.bounds[ 1-ray.sign[1] ].Y;

  float znear = box.bounds[ ray.sign[2] ].Z; 
  float zfar = box.bounds[ 1-ray.sign[2] ].Z;

  float tmin = ( xnear - ray.start.X ) * ray.inv_dir.X;
  float tmax = ( xfar - ray.start.X ) * ray.inv_dir.X;

  float ytmin = ( ynear - ray.start.Y ) * ray.inv_dir.Y;
  float ytmax = ( yfar - ray.start.Y ) * ray.inv_dir.Y;
  
  // Check if ray lies inside the bounds
  if ( ytmin > tmax || tmin > ytmax ){ return false; }
  
  if ( ytmin > tmin ){ tmin = ytmin; } // Choose max of the two tmin values
  if ( ytmax < tmax ){ tmax = ytmax; } // Chost min of the two tmax values
  
  // this way tmin and tmax lie at the boundary of the AABB

  float ztmin = ( znear - ray.start.Z ) * ray.inv_dir.Z;
  float ztmax = ( zfar - ray.start.Z ) * ray.inv_dir.Z;

  if ( ztmin > tmax || tmin > ztmax ){ return false; }

  if ( ztmin > tmin ){ tmin = ztmin; } // Choose max of the two tmin values
  if ( ztmax < tmax ){ tmax = ztmax; } // Chost min of the two tmax values
  
  // tmin < tmax is always true
  return ( ( tmin < t1 ) && ( tmax > t0 ) );
}

int partition(
    void *start,
    int length,
    size_t size,
    int (*filter)(void *) )
{
  int left = 0;
  int right = length;
  char temp[1024];
  assert( sizeof( temp ) >= size );
  for ( int i = 0; i < length; i++ ){
    char *current = ( char *)start + i * size;
    if ( filter( (void *)current) ){
      //swap with the leftmost element
      char *l = (char *)start + left *size;
      char *c = (char *)start + i * size;
      memcpy( temp, l , size );
      memcpy( l, c, size );
      memcpy( c, temp, size );
      left++;
    }
  }
  return left;
}

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
  
  for ( int i = 0; i < 26; i++ ){
    fprintf( stdout,"KEY_CALLBACK_CASE(%c)\n", 'A' + i );
  }
  return 0;

}
2.060608 0.000000 0.000000 0.000000 
0.000000 2.747478 0.000000 0.000000 
0.000000 0.000000 -1.020202 -1.000000 
0.000000 -1.373739 4.898990 5.000000 

The inverse is: 
0.485294 -0.000000 -0.000000 0.000000 
-0.000000 0.363970 0.000000 -0.000000 
-0.000000 0.000000 -0.000000 -4.950000 
0.000000 -0.000000 -1.000000 5.050000 
KEY_CALLBACK_CASE(A)
KEY_CALLBACK_CASE(B)
KEY_CALLBACK_CASE(C)
KEY_CALLBACK_CASE(D)
KEY_CALLBACK_CASE(E)
KEY_CALLBACK_CASE(F)
KEY_CALLBACK_CASE(G)
KEY_CALLBACK_CASE(H)
KEY_CALLBACK_CASE(I)
KEY_CALLBACK_CASE(J)
KEY_CALLBACK_CASE(K)
KEY_CALLBACK_CASE(L)
KEY_CALLBACK_CASE(M)
KEY_CALLBACK_CASE(N)
KEY_CALLBACK_CASE(O)
KEY_CALLBACK_CASE(P)
KEY_CALLBACK_CASE(Q)
KEY_CALLBACK_CASE(R)
KEY_CALLBACK_CASE(S)
KEY_CALLBACK_CASE(T)
KEY_CALLBACK_CASE(U)
KEY_CALLBACK_CASE(V)
KEY_CALLBACK_CASE(W)
KEY_CALLBACK_CASE(X)
KEY_CALLBACK_CASE(Y)
KEY_CALLBACK_CASE(Z)
