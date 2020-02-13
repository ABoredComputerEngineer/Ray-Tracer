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

struct Plane {
  v3 p;
  v3 n;
  Material *m;
  AABB box;
  Plane (): m(NULL){}

  Plane ( v3 point, v3 normal ):p(point), n(normal){}

  Plane ( v3 point, v3 normal, Material *x ):
    p(point), n(normal), m(x){}

  Plane ( v3 p0, v3 p1, v3 p3 ):
    p(p0),
    n( HMM_Cross( p1-p0, p3-p0 ) ){}
};
#endif
