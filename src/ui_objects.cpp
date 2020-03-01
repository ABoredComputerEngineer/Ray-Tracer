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
  if ( AABB_hit( c.bounds, ray, tmin, tmax, record ) ){
    record.obj_type = OBJECT_CUBE;
    record.object = (void *)&c;
    return true;
  }
  return false;
}

AABB cube_get_AABB( void *c ){
  Cube *cube = ( Cube * )c;
  v3 min = { FLT_MAX, FLT_MAX, FLT_MAX };
  v3 max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
  for ( int i = 0; i < 8 ; i++ ){
    v4 vertex = cube->base_transform * DefaultCubeAABBVertex[i];
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
  cube.bounds = cube_get_AABB( &cube );
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
  if ( hit_AARect( g.rect, ray, tmin, tmax, record ) ){
    record.obj_type = OBJECT_GRID;
    record.object = (void *)&g;
    return true;
  }
  return false;
}

v3 grid_get_corner_point( const Grid &grid, f32 u, f32 v ){
  int32 nu = Float2Int( u / grid.unit_u );
  int32 nv = Float2Int( v / grid.unit_v );
  return grid.rect.corner +
         nu*grid.w*grid.dir1 +
         nv*grid.w*grid.dir2;
}

