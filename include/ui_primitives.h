#ifndef UI_PRIMITIVES_H
#define UI_PRIMITIVES_H
#include <float.h>
#include "HandmadeMath.h"
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

union AABB{
  enum {
    LOWER = 0,
    UPPER = 1,
  };
  v3 bounds[2];
  struct {
    v3 l,u;
  };

  AABB () {
    bounds[0] = v3{ FLT_MAX, FLT_MAX, FLT_MAX }; 
    bounds[1] = v3{ -FLT_MAX, -FLT_MAX, -FLT_MAX }; 
  }

  AABB (v3 lower, v3 upper){
    bounds[UPPER] = upper;
    bounds[LOWER]=lower;
  }

};

struct Sphere {
  v3 c;
  float r;
  AABB box;
  Sphere ( v3 center, float radius ):c(center), r(radius){}
};

AABB sphere_aabb( const Sphere &sph ){
  v3 r = { sph.r, sph.r, sph.r };
  return AABB( sph.c - r, sph.c + r );
}

AABB sphere_aabb( Sphere *sph ){
  v3 r = { sph->r, sph->r, sph->r };
  return AABB( sph->c - r, sph->c + r );
}
struct Plane {
  v3 p;
  v3 n;
  AABB box;
  Plane (){ }

  Plane ( v3 point, v3 normal ):p(point), n(normal){}

  Plane ( v3 point, v3 normal, AABB b ):
    p(point), n(normal){ box = b; }

  Plane ( v3 p0, v3 p1, v3 p3, AABB &b ):
    p(p0),
    n( HMM_Cross( p1-p0, p3-p0 ) ){
      box = b;
    }
};

struct AARect{
  enum RectType {
    PLANE_XY = 0,
    PLANE_YZ = 1,
    PLANE_ZX = 2
  };
  RectType type;
  float d;
  v3 n;
  AABB bounds;
  int ndim, d0,d1;
  v3 corner;
  AARect(){}
  AARect ( RectType t, float dist, AABB b,int flip  ):
    type(t),d(dist), bounds(b), d0( t ), d1( (t+1)%3 )
  {
    int sign = ( flip )?-1:1;
    switch ( t ){
      case PLANE_XY:
        n = v3{ 0.0f, 0.0f, sign * 1.0f };
        ndim = 2;
        corner = v3{ b.l[0], b.l[1],d};
        break;
      case PLANE_YZ:
        n = v3{ sign*1.0f,0.0f, 0.0f };
        ndim = 0;
        corner = v3{ d, b.l[1], b.l[2] };
        break;
      case PLANE_ZX:
        n = v3{ 0.0f, sign*1.0f, 0.0f };
        ndim = 1;
        corner = v3{ b.l[0], d, b.l[2] };
        break;
    }
    bounds.l[ ndim ] = d - 0.001f;
    bounds.u[ ndim ] = d + 0.001f;
  }
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

struct HitRecord{
  float t;
  v3 p;
  v3 n;
  float u,v;

  void print( ){
    fprintf( stdout, "Hit Record Info\n" );
    fprintf( stdout, "Parameter t = %f\n", t );
    fprintf( stdout, "Point p = " );
    print_v3( p );
    fprintf( stdout, "\nNormal n = " );
    print_v3( n );
    fprintf( stdout, "\nu = %f, v = %f\n",u,v );
  }
};

bool hit_AARect(
    const AARect &rect,
    const Ray &ray,
    float tmin,
    float tmax,
    HitRecord &record )
{

  //if ( fabs( ray.direction[ rect.ndim ] ) < TOLERANCE ) return false;
  // TODO: We MOST probably don't need a dot product here! Simple multiplication
  // should do
  //float check = HMM_DotVec3( ray.direction, rect.n );
  //if ( check > 0.0f ) return false;
  float t = ( rect.d - ray.start[rect.ndim] )/ray.direction[rect.ndim];
  if ( t < tmin || t > tmax ) return false; 
  float a = ray.start[ rect.d0 ] + t * ray.direction[ rect.d0 ];
  float b = ray.start[ rect.d1 ] + t * ray.direction[ rect.d1 ];

 
  if ( a < rect.bounds.l[ rect.d0 ] || a > rect.bounds.u[ rect.d0 ] ){
    return false;
  }
  if ( b < rect.bounds.l[ rect.d1 ] || b > rect.bounds.u[ rect.d1 ] ){
    return false;
  }

  // UV calculation
  float lower0 = rect.bounds.l[ rect.d0 ];
  float lower1 = rect.bounds.l[ rect.d1 ];
  record.u = ( a - lower0 )/( rect.bounds.u[rect.d0] - rect.bounds.l[rect.d0] );
  record.v = ( b - lower1 )/( rect.bounds.u[rect.d1] - rect.bounds.l[rect.d1] );

  record.p[rect.d0] = a;
  record.p[rect.d1] = b;
  record.p[rect.ndim] = rect.d;
  record.n = rect.n;
  record.t = t;
  return true;

}
#endif
