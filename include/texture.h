#ifndef RAY_TEXTURE_H
#define RAY_TEXTURE_H

#include "prng.h"
#include "common.h"
#include "HandmadeMath.h"
struct Texture;

struct Perlin {
  Arena *store;
  float *random_floats;
  int *permx,*permy,*permz;
  int n;
  float scale;
};


typedef v3 ( *ColorFunc )(
    Texture *texture,
    float u,
    float v,
    const v3 &p );

float perlin_get_noise(Perlin *p, const v3 &point );
struct Texture {
  enum TextureType {
    NONE,
    PLAIN_COLOR,
    CHECKER,
    PERLIN_NOISE
  };
  
  TextureType type;
  ColorFunc get_color;
  union {
    v3 color;
    struct {
      v3 checker_color[2];
    };

    struct {
      Perlin *perlin;
    };

  };

  Texture (): type(NONE), get_color(NULL){}
};

v3 get_plain_tex_color( Texture *tex, float u, float v, const v3 &p ){
  return tex->color;
}

v3 get_checker_tex_color( Texture *tex, float u, float v, const v3 &p ){
  int x = ( sin(p.X*10)  * sin(p.Y*10) * sin( p.Z *10 ) ) < 0;
  return tex->checker_color[x];
}

v3 get_perlin_color( Texture *tex, float u, float v, const v3 &p ){
  Perlin *per = tex->perlin;
  float c = perlin_get_noise( per, p );
  return c * v3{ 1.0f, 1.0f, 1.0f };
}

v3 texture_get_color( Texture *tex, float u, float v, const v3 &p ){
  switch ( tex->type ){
    case Texture::PLAIN_COLOR:
      return tex->color;
      break;
    case Texture::NONE:
    default:
      return v3{0,0,0};
      break;
  }
}

Texture create_texture_plain( v3 color ){
  Texture tex;
  tex.type = Texture::PLAIN_COLOR;
  tex.get_color = get_plain_tex_color;
  tex.color = color;
  return tex;
}

Texture create_texture_checker( v3 c0, v3 c1 ){
  Texture tex;
  tex.type = Texture::CHECKER;
  tex.get_color = get_checker_tex_color;
  tex.checker_color[0] = c0;
  tex.checker_color[1] = c1;
  return tex;
}

float *random_float_array( float *mem, int n ){
  for ( int i = 0; i < n ; i++ ){
    mem[i] = prng_float();
  }
  return mem;
}

template <typename T>
void permute_array( T *list, int n ){
  for ( int i = n-1; i >= 0 ; i-- ){
    // Choose a random index between 0 and -1
    int index = (int)( prng_float() * (i+1) );
    // swap the value at current index with the value at random index
    T temp = list[i];
    list[i] = list[ index ];
    list[index] =temp;
  }
}


Perlin create_perlin( Arena *arena, float scale, int n){
  Perlin p;
  p.store = arena;
  p.scale = scale;
  p.n = n;

  p.random_floats = random_float_array(
      (float *)arena_alloc( p.store, sizeof(float)* p.n,8  ),
      p.n 
  );
  p.permx = (int *)arena_alloc( p.store, sizeof(float)*p.n,8 );
  p.permy = (int *)arena_alloc( p.store, sizeof(float)*p.n,8 );
  p.permz = (int *)arena_alloc( p.store, sizeof(float)*p.n,8 );

  for ( int i = 0; i < p.n; i++ ){
    p.permx[i] = i;
    p.permy[i] = i;
    p.permz[i] = i;
  }

  permute_array( p.permx, p.n );
  permute_array( p.permy, p.n );
  permute_array( p.permz, p.n );
  return p;
}

float perlin_get_noise(Perlin *perlin, const v3 &point ){
  v3 p=perlin->scale * point;

  
  v3 t = {
    p.X - Float2Int( p.X ),
    p.Y - Float2Int( p.Y ),
    p.Z - Float2Int( p.Z )
  };

  t= t*t-(3-2*t);

	int start[3]= {
   	Float2Int( p.X ) & (perlin->n -1 ),
   	Float2Int( p.Y )& (perlin->n -1 ),
   	Float2Int( p.Z ) & (perlin->n -1 )
	};
	
	float arr[8];
  float *s = arr;
	for ( int i = 0; i < 2; i++ ){
    int p2 = ( start[2] + i  ) & ( 255 );

		for ( int j = 0; j < 2; j++ ){
      int p1 = ( start[1] + j  ) & ( 255 );

      for ( int k = 0; k < 2; k++ ){
        int p0 = ( start[0] + k  ) & ( 255 );

        *s = perlin->random_floats[
          perlin->permx[ p0 ] ^ perlin->permy[ p1 ] ^ perlin->permz[ p2 ]
         ];
        s++;

      }
    }
	}

  int n= 8;
  for ( int i = 0; i < 3 ; i++ ){
    for ( int j = 0; j < n; j+=2 ){
      arr[j>>1] = HMM_Lerp( arr[j], t[i], arr[j+1] );
    }
    n >>= 1;
  }

  return arr[0];
  

}

Texture create_texture_perlin( Perlin *p ){
  Texture t;
  t.type = Texture::PERLIN_NOISE;
  t.get_color = get_perlin_color;
  t.perlin = p;
  return t;
}

#endif
