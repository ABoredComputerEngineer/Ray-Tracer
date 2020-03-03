#ifndef RAY_DATA_H
#define RAY_DATA_H
#include "ui_primitives.h"
#include "HandmadeMath.h"

struct DumpCameraData {
  v3 look_from;
  v3 look_at;
  float z;
  float vfov;
  float aspect_ratio;
  float aperture;
  float focal_dist;
};


struct DumpObjectData {
  enum DumpObjectType : uint8 {
    RECTANGLE = 0, 
    SPHERE = 1
  };

  enum DumpMaterialType : uint8 {
    MATERIAL_PURE_DIFFUSE = 0,
    MATERIAL_METALLIC,
    MATERIAL_GLASS,
    MATERIAL_DIFFUSE_LIGHT,
    MATERIAL_SPOT_LIGHT
  };

  enum DumpTextureType: uint8 {
    TEXTURE_NONE,
    TEXTURE_PLAIN_COLOR,
    TEXTURE_CHECKER,
    TEXTURE_PERLIN_NOISE,
    TEXTURE_MARBLE
  };

  uint8 type;

  Sphere sphere;
  Rectangle r;

  uint8 material_type;
  uint8 texture_type;
  union {
    struct {
      float fuzz;
    };

    struct {
      float ri; // w.r.to air
    };

    struct {
      v3 diff_light_color;
    };

    struct {
      v3 spot_light_color;
      float angle;
    };
  } material_data;

  union {
    v3 color;

    struct {
      v3 checker_color[2];
    };
    struct {
      v3 marble_color;
    };
  } texture_data;

};
#endif
