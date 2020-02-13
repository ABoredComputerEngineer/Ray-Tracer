#include <stdio.h>
#include <inttypes.h>
#define OS_LINUX_CPP
#define HANDMADE_MATH_IMPLEMENTATION
#include <float.h>
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

int main( void ){
  v3 near = { -1, -1, -1 };
  v3 far = { -5, -5 , -5 };
  AABB box( far, near);
  Ray r( v3{-2,-2,-2},v3{ 1, 1, 1 } );

  int x = AABB_hit( box, r, 0.0f, FLT_MAX );
  printf( "%d\n", x );
  return 0;

}
