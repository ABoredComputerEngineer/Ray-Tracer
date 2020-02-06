#define HANDMADE_MATH_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define OS_LINUX_CPP
#include <stdio.h>
#include <inttypes.h>
#include <HandmadeMath.h>
#include <assert.h>
#include <prng.h>
#include <common.h>
#include <stb_image_write.h>
#define TOLERANCE 0.000001f
#define ANTI_ALIASING_ON 
typedef hmm_vec2 v2;
typedef hmm_vec3 v3;
typedef hmm_vec4 v4;
typedef hmm_mat4 m4;
typedef unsigned int uint;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;



inline bool refract( const v3 &n, const v3 &v, float ri, v3 &ret ){
  v3 unit_v = HMM_NormalizeVec3( v );
  float angle = ( HMM_DotVec3( n, unit_v ) );
  float dis = 1.0f - ( ri * ri )*( 1.0f - angle * angle );
  if ( dis < 0.0f ) return false;
  ret = ri*(unit_v-angle*n) - sqrt( dis ) * n;
  return true;
}

inline float schlick( float c, float ri ){
  // Gives a parameter that is used to decide whether the 
  // light is supposed to reflect of refract
  float r0 = ( 1-ri )/(1+ri);
  r0 *= r0;
  return r0 + ( 1-r0 )*pow( 1-c, 5 );
}

v3 random_in_unit_disk( ){
  v3 y;
  float dist;
  do {
    v3 p = { prng_float(), prng_float(), 0.0f };
    y = 2 * p - (v3){1.0f,1.0f,0.0f};
    dist = HMM_DotVec3( p, p );
  } while( dist >= 1.0f );
  return y;
}


#if 0

v3 random_in_unit_sphere( void ){
  v3 v = { prng_float(), prng_float(), prng_float() };
  float m = HMM_DotVec3( v, v );
  if ( m < 1.0f ) return v;
  return HMM_NormalizeVec3( v )/m;
}

#else
v3 random_in_unit_sphere( void ){
  float dist;
  v3 p,y;
  do {
    // Generate a random point between (-1,-1,-1 ) and ( 1,1,1 )
    p = { prng_float(),prng_float(), prng_float()}; 
    y = (v3){ -1.0f, -1.0f, -1.0f } + 2 * p;
    dist = HMM_DotVec3( y , y );
  } while ( dist >= 1.0f );
  return y;
}

#endif
struct Material;

struct Ray {
  v3 start;
  v3 direction;

  inline v3 point_at( float t ) const{
    return start + t*(direction);
  }

  Ray ( v3 origin, v3 dir ):start(origin), direction( dir ) {}
  Ray (){}
};

struct Camera {
  v3 origin;
  v3 lower_left;
  v3 front, right, up;
  v3 horizontal;
  v3 vertical;
  float lens_radius; 

  Camera ( float vfov,float aspect_ratio ){ 
    float half_height = tan( HMM_ToRadians(vfov)/2 );
    float half_width = aspect_ratio * half_height;
    lower_left = { -half_width, -half_height, -1.0f};
    horizontal = { 2*half_width, 0.0f, 0.0f };
    vertical = { 0.0f, 2*half_height, 0.0f };
    origin = { 0.0f, 0.0f, 0.0f };
  }

  Camera (
      v3 look_from,
      v3 look_at,
      float vfov,
      float aspect_ratio,
      float aperture,
      float focal_dist)
  {
    float fd = focal_dist;
    origin = look_from;
    float half_height = tan( HMM_ToRadians(vfov)/2 );
    float half_width = aspect_ratio * half_height;
    
    front = HMM_NormalizeVec3( look_from-look_at );   
    v3 aup = { 0.0f, 1.0f, 0.0 };

    right = HMM_Cross( aup, front );
    up = HMM_Cross( front, right );

    lower_left =origin-fd*front-right*fd* half_width-up*fd*half_height;
    lens_radius = aperture/2;
    horizontal = 2 * half_width*fd * right;
    vertical = 2 * half_height *fd* up;
  }

  inline Ray get_ray( float u, float v ){
    v3 x = lens_radius * random_in_unit_disk();
    v3 off = x.X * right + x.Y * up;
    v3 start = origin + off;
    return Ray( start, lower_left - start + u * horizontal+ v *vertical);
  }
};
struct HitRecord{
  float t;
  v3 p;
  v3 n;
  Material *m;
};

enum MaterialType {
  MATERIAL_PURE_DIFFUSE,
  MATERIAL_DIFFUSE,
  MATERIAL_METALLIC,
  MATERIAL_GLASS,
};


typedef bool (*ScatterFunc)(
    const HitRecord &h,
    const Ray &in,
    v3 &attenuation,
    Ray &out );

struct Sphere {
  v3 c;
  float r;
  Material *m;
  Sphere ( v3 center, float radius ):c(center), r(radius){}

  Sphere ( v3 center, float radius, Material *x ):
    c(center), r(radius), m(x){}
};

struct Plane {
  v3 p;
  v3 n;
  Material *m;

  Plane (): m(NULL){}

  Plane ( v3 point, v3 normal ):p(point), n(normal){}

  Plane ( v3 point, v3 normal, Material *x ):
    p(point), n(normal), m(x){}

  Plane ( v3 p0, v3 p1, v3 p3 ):
    p(p0),
    n( HMM_Cross( p1-p0, p3-p0 ) ){}
};

struct World {
  Sphere *spheres;
  uint sph_count;
  uint sph_cap;

  Plane *planes;
  uint plane_count;
  uint plane_cap;
};




struct Material {
  MaterialType type;
  ScatterFunc scatter;
  v3 albedo;
  union {
    struct {
      float fuzz;
    };

    struct {
      float ri; // w.r.to air
    };
  };

  Material ( MaterialType t, ScatterFunc func,v3 a, float f ):
    type(t), scatter( func ), albedo( a )
  {
    switch ( t ){
      case MATERIAL_PURE_DIFFUSE:
      case MATERIAL_DIFFUSE:
        break;
      case MATERIAL_METALLIC:
        fuzz = f; 
        break;
      case MATERIAL_GLASS:
        ri = f;
        break;
      default:
        fprintf( stderr, "Unknown material type!" );
        assert( 0 );
        break;
    }
  }
};


bool hit_plane(
    Plane &p,
    const Ray &ray,
    float tmin,
    float tmax,
    HitRecord &record )
{
  float d = HMM_DotVec3( ray.direction, p.n );
  if ( abs( d ) < TOLERANCE )
    return false;
#if 0 
  v3 temp = ray.start - p.p;
#else
  v3 temp = p.p - ray.start;
#endif
  float t = HMM_DotVec3( temp, p.n )/d ;
  if ( t > tmin && t < tmax ){
    record.t = t;
    record.p = ray.point_at( t );
    record.n = p.n;
    record.m = p.m;
    return true;
  } 
  return false;
}

bool pure_diffuse_scatter(
    const HitRecord &rec,
    const Ray &in,
    v3 &attenuation,
    Ray &out )
{
  v3 dir = rec.n + random_in_unit_sphere();
  out = Ray( rec.p , dir );
  attenuation = rec.m->albedo;
  return true;
}


bool metallic_scatter(
    const HitRecord &rec,
    const Ray &in,
    v3 &attenuation,
    Ray &out )
{
  v3 dir = HMM_Reflect( HMM_NormalizeVec3(in.direction), rec.n ) +
           rec.m->fuzz * random_in_unit_sphere();
  out = Ray( rec.p, dir );
  attenuation = rec.m->albedo;
  return ( HMM_DotVec3( out.direction, rec.n ) > 0 ) ;
}

bool refraction_scatter(
    const HitRecord &rec,
    const Ray &in,
    v3 &attenuation,
    Ray &out )
{
  Material *m = rec.m;
  float ri;
  attenuation = m->albedo;
  v3 outward_normal;
  float cosine = HMM_DotVec3( HMM_NormalizeVec3( in.direction ), rec.n );
  if ( cosine > 0.0f ){
    // Entering from medium to air
    ri = m->ri; // ( n1=ri, n2 = 1.0f )
    outward_normal = -rec.n;
    cosine *= m->ri ;
  } else {
    ri = 1/m->ri; // ( n1 = 1.0f, n2 = ri )
    outward_normal = rec.n;
    cosine = -cosine;
  }
  
  v3 refract_dir = {0};
  float reflect_prob;
  v3 reflect_dir = HMM_Reflect( HMM_Normalize(in.direction), rec.n ); 
  if ( refract( outward_normal, in.direction,ri, refract_dir ) ){
    // if refraction is possible,
    // calculate the possibility of reflection
    reflect_prob = schlick( cosine, m->ri ); 
  } else {
    // reflect the ray if refraction is not possible
    reflect_prob = 1.0f; 
  }
  
  if ( prng_float() < reflect_prob ){
    out = Ray( rec.p, reflect_dir );
  } else {
    out = Ray( rec.p, refract_dir );
  }

  return true;
}



bool hit_sphere(
    Sphere &sph,
    const Ray &ray,
    float tmin,
    float tmax,
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
      record.m = sph.m;
      return true;
    }

    t = ( -b+sqrt( dis ) )/(2.0f*a);
    if ( t > tmin && t < tmax ){
      record.t = t;
      record.p = ray.point_at( t );
      record.n = HMM_NormalizeVec3( record.p - sph.c );
      record.m = sph.m;
      return true;
    }
  }
  return false;
}

bool world_check_hit(
    World &w,
    const Ray &ray,
    HitRecord &rec )
{
  bool hit_anything = false;
  HitRecord tr;
  float tmin = 0.001f;
  float tmax = 1000.0f;
  for ( uint i = 0; i < w.sph_count; i++ ){
    if ( hit_sphere( w.spheres[i], ray, tmin, tmax, tr )){
      tmax = tr.t;
      hit_anything = true;
      rec = tr;
    }
  }

  for ( uint i = 0; i < w.plane_count; i++ ){
    if ( hit_plane( w.planes[i], ray, tmin, tmax, tr )){
      tmax = tr.t;
      hit_anything = true;
      rec = tr;
    }
  }
  return hit_anything;
}


v3 get_ray_color( World &w,  const Ray &ray, int depth ){
  v3 direction = HMM_NormalizeVec3( ray.direction );
  
  HitRecord rec;
  v3 attn;
  Ray out;
  if ( world_check_hit(w,ray,rec) ){
//    return 0.5f * ( rec.n + (v3){1.0f,1.0f,1.0f} );
    if ( depth < 50 &&  rec.m->scatter( rec, ray, attn, out ) ){
      return attn * get_ray_color( w, out, depth+1 );
    } else {
      return { 0,0,0 };
    }
  }
#if 1
    float t = 0.5f * ( direction.Y + 1.0f );
    
    v3 start= { 1.0f, 1.0f, 1.0f };
    v3 end = { 0.5f, 0.7f, 1.0f };
    
    return ( 1.0 - t ) * start + t * end;
#else
    return { 0.1f, 0.45f, 0.32f };
#endif
}

void world_add_sphere( World &w, const Sphere &sph ){
  assert( w.sph_count + 1 <= w.sph_cap );
  w.spheres[ w.sph_count++ ] = sph;
}

void world_add_plane( World &w, Plane p ){
  assert( w.plane_count + 1 <= w.plane_cap );
  w.planes[ w.plane_count++ ] = p;
}


int main( ){
  prng_seed();
  int nx = 800;
  int ny = 600;
  int samples = 100;
  
  Material mat_pure_diffuse_white( MATERIAL_PURE_DIFFUSE,
                         pure_diffuse_scatter,
                         (v3){ 0.5f, 0.5f, 0.5f }, 0.0f );

  Material mat_pure_diffuse_pink( MATERIAL_PURE_DIFFUSE,
                         pure_diffuse_scatter,
                         (v3){ 0.8f, 0.3f, 0.3f }, 0.0f );

  Material mat_pure_diffuse_blue( MATERIAL_PURE_DIFFUSE,
                         pure_diffuse_scatter,
                         (v3){ 0.1f, 0.2f, 0.5f }, 0.0f );

  Material mat_pure_diffuse_green( MATERIAL_PURE_DIFFUSE,
                         pure_diffuse_scatter,
                         (v3){ 0.8f, 0.8f, 0.0f },0.0f );

  Material mat_metallic( MATERIAL_METALLIC,
                         metallic_scatter,
                         (v3){ 0.8f, 0.8f, 0.8f },
                         0.3f);

  Material mat_pure_metallic( MATERIAL_METALLIC,
                         metallic_scatter,
                         (v3){ 0.8f, 0.8f, 0.8f },
                         0.0f);

  Material mat_pure_glass( MATERIAL_GLASS,
                         refraction_scatter,
                         (v3){ 1.0f, 1.0f, 1.0f },
                         1.5f );
  
  v3 lf = { 2,2,1 };
  v3 lat = { -1.5f,0,-1 };
  float f = HMM_LengthVec3( lat - lf );
  Camera camera(
      lf,
      lat,
      90, ( float )nx/ny,
      0.1f,
      f
      );
  World world = {};
  world.sph_cap = 4;
  world.spheres = ( Sphere * )malloc(
                  sizeof(Sphere )* world.sph_cap );

  world.plane_cap = 4;
  world.planes = ( Plane * )malloc(
                  sizeof(Plane)* world.plane_cap );

  assert( world.spheres );

#if 1
  world_add_sphere( world,
      Sphere( {-1.5f, 0.0f, -1.5f}, 0.5f,&mat_pure_diffuse_blue));
  world_add_sphere( world,
      Sphere( {0.0f, 0.0f, -1.0f}, 0.5f,&mat_pure_metallic));
  world_add_sphere( world,
      Sphere( {1.5f, 0.0f, -1.5f}, 0.5f,&mat_pure_glass));
  //world_add_sphere( world,
  //    Sphere( {0.0f, -100.5f, -1.0f}, 100.0f,&mat_pure_diffuse_green) );

  world_add_plane( world,
                   Plane( (v3){ 0.0f, -0.5f, 0.0f },
                          (v3){ 0.0f, 1.0f, 0.0f },
                          &mat_pure_diffuse_pink)
                 );
#else
  world_add_sphere( world,
      Sphere( {0,0,-1.0f}, 0.5f, &mat_pure_diffuse_green ) );
  world_add_sphere( world,
      Sphere( {0,-100.5f,-1.0f}, 100.0f, &mat_pure_diffuse_pink ) );
  world_add_sphere( world,
      Sphere( {1.0f,0,-1.0f}, 0.5f, &mat_pure_metallic ) );
  world_add_sphere( world,
      Sphere( {-1.0f,0,-1.0f}, 0.5f, &mat_pure_glass ) );
#endif

  uint8 *buff = ( uint8 *)malloc( 3 * nx * ny * sizeof( uint8 ) );
  uint8 *start = buff;
  for ( int j = ny - 1; j >= 0; j-- ){
    for ( int i = 0; i < nx; i++ ){
      v3 color = { 0.0f, 0.0f, 0.0f };
      for ( int k = 0; k < samples ; k++ ){
        float s1 = ( i + prng_float() )/(float)nx;
        float s2 = ( j + prng_float() )/(float)ny;
        Ray r = camera.get_ray( s1, s2 );
        color = color + get_ray_color( world,r, 0 );
      }
      color = color / samples;
      color = { HMM_SquareRootF( color[0] ),
                HMM_SquareRootF( color[1] ),
                HMM_SquareRootF( color[2] ) };

      int ir = (int)(255.99 * color[0] );
      int ig = (int)(255.99 * color[1] );
      int ib = (int)(255.99 * color[2] );
      
      *start++ = ir & 0xff;
      *start++ = ig & 0xff;
      *start++ = ib & 0xff;
    }
  }
  
  stbi_write_png( "./out.png", nx, ny, 3, buff,3 * nx );
  return 0;
}
