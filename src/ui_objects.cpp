#define HANDMADE_MATH_IMPLEMENTATION
#include "ui_objects.h"
static v4 DefaultCubeAABBVertex[] = {
  v4{ 0.5f, 0.5f, 0.5f, 1.0f },
  v4{ -0.5f, 0.5f, 0.5f,1.0f },
  v4{ -0.5f, -0.5f, 0.5f,1.0f },
  v4{ 0.5f, -0.5f, 0.5f, 1.0f},
  v4{ 0.5f, 0.5f, -0.5f, 1.0f },
  v4{ -0.5f, 0.5f, -0.5f, 1.0f },
  v4{ -0.5f, -0.5f, -0.5f,1.0f },
  v4{ 0.5f, -0.5f, -0.5f, 1.0f }
};
Line create_line( const v3 &start, const v3 &end, const v3 &color ){
  Line l = { start, end };
  l.color = color;
  return l;
}

Line create_line_from_ray( const Ray &r, f32 len, const v3 &color ){
  Line l;
  l.start = r.start;
  l.end = r.point_at( len );
  l.color= color;
  return l;
}
bool hit_cube(
    const Cube &c,
    const Ray &ray,
    float tmin, float tmax,
    HitRecord &record )
{
  return AABB_hit( c.bounds, ray, tmin, tmax, record );
}

AABB cube_get_AABB( void *c, const m4 &m ){
  Cube *cube = ( Cube * )c;
  v3 min = { FLT_MAX, FLT_MAX, FLT_MAX };
  v3 max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
  for ( int i = 0; i < 8 ; i++ ){
    v4 vertex = m* DefaultCubeAABBVertex[i];
    min.X = ( min.X < vertex.X )?min.X:vertex.X;
    min.Y = ( min.Y < vertex.Y )?min.Y:vertex.Y;
    min.Z = ( min.Z < vertex.Z )?min.Z:vertex.Z;

    max.X = ( max.X > vertex.X )?max.X:vertex.X;
    max.Y = ( max.Y > vertex.Y )?max.Y:vertex.Y;
    max.Z = ( max.Z > vertex.Z )?max.Z:vertex.Z;
  }


  cube->bounds = AABB( min, max );
  return cube->bounds;
}

Cube create_cube_one_color( float len, v3 pos, v3 color )
{

  Cube cube;
  cube.pos = pos;
  cube.length = len;
  for ( int i = 0; i < 6; i++ ){
    cube.color[i] = color;
  }
  cube.base_transform = HMM_Translate(pos) *
                        HMM_Scale( v3{ len, len, len } ); 
  cube.orientation = HMM_QuaternionFromAxisAngle( v3{1.0f,1.0f,1.0f}, 0.0f );
  cube.bounds = cube_get_AABB( &cube, cube.base_transform );
  return cube;
}

Grid create_grid(
    AARect::RectType type,
    const AABB &b,
    f32 d,
    f32 width,
    const v3 &color )
{
  Grid g;
  g.rect = AARect( type, d, b, 0 ); 
  g.w = width;
  g.color = color;
  switch ( type ){
    case AARect::PLANE_XY:
     g.dir1 = v3{ 1.0f, 0.0f, 0.0f };
     g.dir2 = v3{ 0.0f, 1.0f, 0.0f };
      break;
    case AARect::PLANE_YZ:
     g.dir1 = v3{ 0.0f, 1.0f, 0.0f };
     g.dir2 = v3{ 0.0f, 0.0f, 1.0f };
      break;
    case AARect::PLANE_ZX:
     g.dir1 = v3{ 0.0f, 0.0f, 1.0f };
     g.dir2 = v3{ 1.0f, 0.0f, 0.0f };
      break;
    default:
      print_error( "Unknown Case!\n" );
      break;
  }

  f32 x = g.rect.bounds.u[ g.rect.d0 ];
  f32 y = g.rect.bounds.l[ g.rect.d0 ];

  f32 z = g.rect.bounds.u[ g.rect.d1 ];
  f32 w = g.rect.bounds.l[ g.rect.d1 ];

  // u is along rect.d0, v is along rect.d1
  g.unit_u = g.w / ( x - y );
  g.unit_v = g.w / ( z - w );


  g.nlines = Float2Int( ( g.rect.bounds.u[g.rect.d0]-g.rect.bounds.l[g.rect.d0] )/width );
  return g;
}

bool hit_grid(
    const Grid &g,
    const Ray &ray,
    float tmin, float tmax,
    HitRecord &record )
{
  return hit_AARect( g.rect, ray, tmin, tmax, record );
}

v3 grid_get_corner_point( const Grid &grid, f32 u, f32 v ){
  int32 nu = Float2Int( u / grid.unit_u );
  int32 nv = Float2Int( v / grid.unit_v );
  return grid.rect.corner +
         nu*grid.w*grid.dir1 +
         nv*grid.w*grid.dir2;
}

bool hit_sphere(
    const Sphere &sph,
    const Ray &ray,
    float tmin, float tmax,
    HitRecord &record )
{
  v3 v = ray.start - sph.c;
  float b = 2 * HMM_DotVec3( ray.direction, v );
  float a = HMM_DotVec3( ray.direction, ray.direction );
  float c = HMM_DotVec3( v,v ) - sph.r*sph.r;

  float dis = b * b - 4 * a * c;
  if ( dis > 0 ){
    float t = ( -b-sqrt( dis ) )/(2.0f*a);
    if ( t > tmin && t < tmax ){
      record.t = t;
      record.p = ray.point_at( t );
      record.n = HMM_NormalizeVec3( record.p - sph.c );
      return true;
    }

    t = ( -b+sqrt( dis ) )/(2.0f*a);
    if ( t > tmin && t < tmax ){
      record.t = t;
      record.p = ray.point_at( t );
      record.n = HMM_NormalizeVec3( record.p - sph.c );
      return true;
    }
  }
  return false;
}


