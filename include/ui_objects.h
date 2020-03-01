#include <stdio.h>
#include "ui_primitives.h"
#include "common.h"
struct Line {
  v3 start, end,color;
};
Line create_line( const v3 &start, const v3 &end, const v3 &color );
Line create_line_from_ray( const Ray &r, f32 len, const v3 &color );

struct Grid {
  AARect rect;
  Line l1, l2;
  v3 dir1, dir2;  
  f32 w; // width
  f32 unit_u;
  f32 unit_v;
  v3 color;
  int32 nlines;

  uint vao, vbo, ebo;
};


enum CubeState {
  CUBE_ANIMATING,
  CUBE_STATIC
};

struct TranslateAnimation {
  v3 start;
  v3 end;
};

struct RotateAnimation {
  q4 start;
  q4 end;
  float t;
  uint duration;
  uint elapsed;
  float angle;
  m4 rot;
};

struct Cube {

  enum Faces {
    FRONT = 0,
    BACK,
    RIGHT,
    LEFT,
    TOP,
    BOT
  };
  // Render Info
  uint vao,vbo;
  uint program;

  CubeState state;
  TranslateAnimation tanim;
  RotateAnimation ranim;

  AABB bounds;

  m4 base_transform;
  v3 pos;
  f32 length;

  q4 orientation;
  f32 angle;

  union {
    v3 color[6];
  };

};
bool hit_cube(
    const Cube &c,
    const Ray &ray,
    float tmin, float tmax,
    HitRecord &record );

bool hit_grid(
    const Grid &g,
    const Ray &ray,
    float tmin, float tmax,
    HitRecord &record );

v3 grid_get_corner_point( const Grid &grid, f32 u, f32 v );
Cube create_cube_one_color( float len, v3 pos, v3 color );
Grid create_grid(
    AARect::RectType type,
    const AABB &b,
    f32 d,
    f32 width,
    const v3 &color );
