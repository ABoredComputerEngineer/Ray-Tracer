#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include "HandmadeMath.h"

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
struct Material;

struct Sphere {
  v3 c;
  float r;
  Material *m;
  AABB box;
  Sphere ( v3 center, float radius ):c(center), r(radius){}

  Sphere ( v3 center, float radius, Material *x ):
    c(center), r(radius), m(x){}

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
  Material *m;
  AABB box;
  Plane (): m(NULL){}

  Plane ( v3 point, v3 normal ):p(point), n(normal){}

  Plane ( v3 point, v3 normal, Material *x, AABB b ):
    p(point), n(normal), m(x){ box = b; }

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
  Material *m;
  AARect(){}
  AARect ( RectType t, float dist, AABB b,int flip, Material *mat ):
    type(t),d(dist), bounds(b), d0( t ), d1( (t+1)%3 ), m( mat )
  {
    int sign = ( flip )?-1:1;
    switch ( t ){
      case PLANE_XY:
        n = v3{ 0.0f, 0.0f, sign * 1.0f };
        ndim = 2;
        break;
      case PLANE_YZ:
        n = v3{ sign*1.0f,0.0f, 0.0f };
        ndim = 0;
        break;
      case PLANE_ZX:
        n = v3{ 0.0f, sign*1.0f, 0.0f };
        ndim = 1;
        break;
    }
    bounds.l[ ndim ] = d - 0.01f;
    bounds.u[ ndim ] = d + 0.01f;
  }
};


#endif
