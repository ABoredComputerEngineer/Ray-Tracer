#ifndef HANDMADE_MATH_ZZ
#define HANDMADE_MATH_ZZ
#include <stdio.h>
/*
  HandmadeMath.h v1.9.0

  This is a single header file with a bunch of useful functions for game and
  graphics math operations.

  =============================================================================

  You MUST

     #define HANDMADE_MATH_IMPLEMENTATION

  in EXACTLY one C or C++ file that includes this header, BEFORE the
  include, like this:

     #define HANDMADE_MATH_IMPLEMENTATION
     #include "HandmadeMath.h"

  All other files should just #include "HandmadeMath.h" without the #define.

  =============================================================================

  To disable SSE intrinsics, you MUST

  #define HANDMADE_MATH_NO_SSE

  in EXACTLY one C or C++ file that includes this header, BEFORE the
  include, like this:

     #define HANDMADE_MATH_IMPLEMENTATION
     #define HANDMADE_MATH_NO_SSE
     #include "HandmadeMath.h"

  =============================================================================

  To use HandmadeMath without the CRT, you MUST

     #define HMM_SINF MySinF
     #define HMM_COSF MyCosF
     #define HMM_TANF MyTanF
     #define HMM_SQRTF MySqrtF
     #define HMM_EXPF MyExpF
     #define HMM_LOGF MyLogF
     #define HMM_ACOSF MyACosF
     #define HMM_ATANF MyATanF
     #define HMM_ATAN2F MYATan2F

  Provide your own implementations of SinF, CosF, TanF, ACosF, ATanF, ATan2F,
  ExpF, and LogF in EXACTLY one C or C++ file that includes this header,
  BEFORE the include, like this:

     #define HMM_SINF MySinF
     #define HMM_COSF MyCosF
     #define HMM_TANF MyTanF
     #define HMM_SQRTF MySqrtF
     #define HMM_EXPF MyExpF
     #define HMM_LOGF MyLogF
     #define HMM_ACOSF MyACosF
     #define HMM_ATANF MyATanF
     #define HMM_ATAN2F MyATan2F
     #define HANDMADE_MATH_IMPLEMENTATION
     #include "HandmadeMath.h"

  If you do not define all of these, HandmadeMath.h will use the
  versions of these functions that are provided by the CRT.

  =============================================================================

  LICENSE

  This software is in the public domain. Where that dedication is not
  recognized, you are granted a perpetual, irrevocable license to copy,
  distribute, and modify this file as you see fit.

  CREDITS

  Written by Zakary Strange (zak@strangedev.net && @strangezak)

  Functionality:
   Matt Mascarenhas (@miblo_)
   Aleph
   FieryDrake (@fierydrake)
   Gingerbill (@TheGingerBill)
   Ben Visness (@bvisness)
   Trinton Bullard (@Peliex_Dev)
   @AntonDan

  Fixes:
   Jeroen van Rijn (@J_vanRijn)
   Kiljacken (@Kiljacken)
   Insofaras (@insofaras)
   Daniel Gibson (@DanielGibson)
*/

// Dummy macros for when test framework is not present.
#ifndef COVERAGE
#define COVERAGE(a, b)
#endif

#ifndef ASSERT_COVERED
#define ASSERT_COVERED(a)
#endif

/* let's figure out if SSE is really available (unless disabled anyway)
   (it isn't on non-x86/x86_64 platforms or even x86 without explicit SSE support)
   => only use "#ifdef HANDMADE_MATH__USE_SSE" to check for SSE support below this block! */
#ifndef HANDMADE_MATH_NO_SSE

# ifdef _MSC_VER
   /* MSVC supports SSE in amd64 mode or _M_IX86_FP >= 1 (2 means SSE2) */
#  if defined(_M_AMD64) || ( defined(_M_IX86_FP) && _M_IX86_FP >= 1 )
#   define HANDMADE_MATH__USE_SSE 1
#  endif
# else /* not MSVC, probably GCC, clang, icc or something that doesn't support SSE anyway */
#  ifdef __SSE__ /* they #define __SSE__ if it's supported */
#   define HANDMADE_MATH__USE_SSE 1
#  endif /*  __SSE__ */
# endif /* not _MSC_VER */

#endif /* #ifndef HANDMADE_MATH_NO_SSE */

#include <stdint.h> // This is for types

#ifdef HANDMADE_MATH__USE_SSE
#include <xmmintrin.h>
#include <x86intrin.h>
#endif

#ifndef HANDMADE_MATH_H
#define HANDMADE_MATH_H

#ifdef _MSC_VER
#pragma warning(disable:4201)
#endif

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#endif

#if defined(__GNUC__) && (__GNUC__ == 4 && __GNUC_MINOR__ < 8)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define HMM_INLINE static inline
#define HMM_EXTERN extern

#if !defined(HMM_SINF) || !defined(HMM_COSF) || !defined(HMM_TANF) || \
    !defined(HMM_SQRTF) || !defined(HMM_EXPF) || !defined(HMM_LOGF) || \
    !defined(HMM_ACOSF) || !defined(HMM_ATANF)|| !defined(HMM_ATAN2F)
#include <math.h>
#endif

#ifndef HMM_SINF
#define HMM_SINF sinf
#endif

#ifndef HMM_COSF
#define HMM_COSF cosf
#endif

#ifndef HMM_TANF
#define HMM_TANF tanf
#endif

#ifndef HMM_SQRTF
#define HMM_SQRTF sqrtf
#endif

#ifndef HMM_EXPF
#define HMM_EXPF expf
#endif

#ifndef HMM_LOGF
#define HMM_LOGF logf
#endif

#ifndef HMM_ACOSF
#define HMM_ACOSF acosf
#endif

#ifndef HMM_ATANF
#define HMM_ATANF atanf
#endif

#ifndef HMM_ATAN2F
#define HMM_ATAN2F atan2f
#endif

#define HMM_PI32 3.14159265359f
#define HMM_PI 3.14159265358979323846

#define HMM_MIN(a, b) (a) > (b) ? (b) : (a)
#define HMM_MAX(a, b) (a) < (b) ? (b) : (a)
#define HMM_ABS(a) ((a) > 0 ? (a) : -(a))
#define HMM_MOD(a, m) ((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m))
#define HMM_SQUARE(x) ((x) * (x))

typedef union hmm_vec2
{
    struct
    {
        float X, Y;
    };

    struct
    {
        float U, V;
    };

    struct
    {
        float Left, Right;
    };

    struct
    {
        float Width, Height;
    };

    float Elements[2];

#ifdef __cplusplus
    inline float &operator[](const int &Index)
    {
        return Elements[Index];
    }
#endif
} hmm_vec2;

typedef union hmm_vec3
{
    struct
    {
        float X, Y, Z;
    };

    struct
    {
        float U, V, W;
    };

    struct
    {
        float R, G, B;
    };

    struct
    {
        hmm_vec2 XY;
        float Ignored0_;
    };

    struct
    {
        float Ignored1_;
        hmm_vec2 YZ;
    };

    struct
    {
        hmm_vec2 UV;
        float Ignored2_;
    };

    struct
    {
        float Ignored3_;
        hmm_vec2 VW;
    };

    float Elements[3];

#ifdef __cplusplus
    inline float &operator[](const int &Index) 
    {
        return Elements[Index];
    }
    
    inline const float &operator[](const int &Index) const 
    {
        return Elements[Index];
    }
#endif
} hmm_vec3;

typedef union hmm_vec4
{
    struct
    {
        union
        {
            hmm_vec3 XYZ;
            struct
            {
                float X, Y, Z;
            };
        };

        float W;
    };
    struct
    {
        union
        {
            hmm_vec3 RGB;
            struct
            {
                float R, G, B;
            };
        };

        float A;
    };

    struct
    {
        hmm_vec2 XY;
        float Ignored0_;
        float Ignored1_;
    };

    struct
    {
        float Ignored2_;
        hmm_vec2 YZ;
        float Ignored3_;
    };

    struct
    {
        float Ignored4_;
        float Ignored5_;
        hmm_vec2 ZW;
    };

    float Elements[4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 InternalElementsSSE;
#endif

#ifdef __cplusplus
    inline float &operator[](const int &Index)
    {
        return Elements[Index];
    }
#endif
} hmm_vec4;

typedef union hmm_mat4
{
    float Elements[4][4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 Columns[4];

    // DEPRECATED. Our matrices are column-major, so this was named
    // incorrectly. Use Columns instead.
    __m128 Rows[4];
#endif

#ifdef __cplusplus
    inline hmm_vec4 operator[](const int &Index)
    {
        float* col = Elements[Index];

        hmm_vec4 result;
        result.Elements[0] = col[0];
        result.Elements[1] = col[1];
        result.Elements[2] = col[2];
        result.Elements[3] = col[3];

        return result;
    }
#endif
} hmm_mat4;

typedef union hmm_quaternion
{
    struct
    {
        union
        {
            hmm_vec3 XYZ;
            struct
            {
                float X, Y, Z;
            };
        };

        float W;
    };

    float Elements[4];

#ifdef HANDMADE_MATH__USE_SSE
    __m128 InternalElementsSSE;
#endif
} hmm_quaternion;

typedef int32_t hmm_bool;

typedef hmm_vec2 hmm_v2;
typedef hmm_vec3 hmm_v3;
typedef hmm_vec4 hmm_v4;
typedef hmm_mat4 hmm_m4;


/*
 * Floating-point math functions
 */

HMM_INLINE float HMM_SinF(float Radians);


HMM_INLINE float HMM_CosF(float Radians);


HMM_INLINE float HMM_TanF(float Radians);


HMM_INLINE float HMM_ACosF(float Radians);


HMM_INLINE float HMM_ATanF(float Radians);


HMM_INLINE float HMM_ATan2F(float Left, float Right);


HMM_INLINE float HMM_ExpF(float Float);


HMM_INLINE float HMM_LogF(float Float);


HMM_INLINE float HMM_SquareRootF(float Float);


HMM_INLINE float HMM_RSquareRootF(float Float);


HMM_EXTERN float HMM_Power(float Base, int Exponent);

HMM_INLINE float HMM_PowerF(float Base, float Exponent);



/*
 * Utility functions
 */

HMM_INLINE float HMM_ToRadians(float Degrees);


HMM_INLINE float HMM_Lerp(float A, float Time, float B);


HMM_INLINE float HMM_Clamp(float Min, float Value, float Max);



/*
 * Vector initialization
 */

HMM_INLINE hmm_vec2 HMM_Vec2(float X, float Y);


HMM_INLINE hmm_vec2 HMM_Vec2i(int X, int Y);


HMM_INLINE hmm_vec3 HMM_Vec3(float X, float Y, float Z);


HMM_INLINE hmm_vec3 HMM_Vec3i(int X, int Y, int Z);


HMM_INLINE hmm_vec4 HMM_Vec4(float X, float Y, float Z, float W);


HMM_INLINE hmm_vec4 HMM_Vec4i(int X, int Y, int Z, int W);


HMM_INLINE hmm_vec4 HMM_Vec4v(const hmm_vec3& Vector, float W);



/*
 * Binary vector operations
 */
HMM_INLINE hmm_vec2 HMM_AddVec2(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 HMM_AddVec3(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 HMM_AddVec4(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_vec2 HMM_SubtractVec2(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 HMM_SubtractVec3(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 HMM_SubtractVec4(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_vec2 HMM_MultiplyVec2(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec2 HMM_MultiplyVec2f(hmm_vec2 Left, float Right);


HMM_INLINE hmm_vec3 HMM_MultiplyVec3(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec3 HMM_MultiplyVec3f(const hmm_vec3& Left, float Right);


HMM_INLINE hmm_vec4 HMM_MultiplyVec4(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_vec4 HMM_MultiplyVec4f(hmm_vec4 Left, float Right);


HMM_INLINE hmm_vec2 HMM_DivideVec2(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec2 HMM_DivideVec2f(hmm_vec2 Left, float Right);


HMM_INLINE hmm_vec3 HMM_DivideVec3(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec3 HMM_DivideVec3f(const hmm_vec3& Left, float Right);


HMM_INLINE hmm_vec4 HMM_DivideVec4(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_vec4 HMM_DivideVec4f(hmm_vec4 Left, float Right);


HMM_INLINE hmm_bool HMM_EqualsVec2(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_bool HMM_EqualsVec3(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_bool HMM_EqualsVec4(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE float HMM_DotVec2(hmm_vec2 VecOne, hmm_vec2 VecTwo);


HMM_INLINE float HMM_DotVec3(const hmm_vec3& VecOne,const hmm_vec3& VecTwo);


HMM_INLINE float HMM_DotVec4(hmm_vec4 VecOne, hmm_vec4 VecTwo);


HMM_INLINE hmm_vec3 HMM_Cross(const hmm_vec3& VecOne,const hmm_vec3& VecTwo);



/*
 * Unary vector operations
 */

HMM_INLINE float HMM_LengthSquaredVec2(hmm_vec2 A);


HMM_INLINE float HMM_LengthSquaredVec3(const hmm_vec3& A);


HMM_INLINE float HMM_LengthSquaredVec4(hmm_vec4 A);


HMM_INLINE float HMM_LengthVec2(hmm_vec2 A);


HMM_INLINE float HMM_LengthVec3(const hmm_vec3& A);


HMM_INLINE float HMM_LengthVec4(hmm_vec4 A);


HMM_INLINE hmm_vec2 HMM_NormalizeVec2(hmm_vec2 A);


HMM_INLINE hmm_vec3 HMM_NormalizeVec3(const hmm_vec3& A);


HMM_INLINE hmm_vec4 HMM_NormalizeVec4(hmm_vec4 A);


HMM_INLINE hmm_vec2 HMM_FastNormalizeVec2(hmm_vec2 A);


HMM_INLINE hmm_vec3 HMM_FastNormalizeVec3(const hmm_vec3& A);


HMM_INLINE hmm_vec4 HMM_FastNormalizeVec4(hmm_vec4 A);



/*
 * SSE stuff
 */

#ifdef HANDMADE_MATH__USE_SSE
HMM_INLINE __m128 HMM_LinearCombineSSE(__m128 Left, hmm_mat4 Right);

#endif


/*
 * Matrix functions
 */

HMM_INLINE hmm_mat4 HMM_Mat4(void);


HMM_INLINE hmm_mat4 HMM_Mat4d(float Diagonal);


#ifdef HANDMADE_MATH__USE_SSE
HMM_INLINE hmm_mat4 HMM_Transpose(hmm_mat4 Matrix);

#else
HMM_EXTERN hmm_mat4 HMM_Transpose(hmm_mat4 Matrix);
#endif

#ifdef HANDMADE_MATH__USE_SSE
HMM_INLINE hmm_mat4 HMM_AddMat4(hmm_mat4 Left, hmm_mat4 Right);

#else
HMM_EXTERN hmm_mat4 HMM_AddMat4(hmm_mat4 Left, hmm_mat4 Right);
#endif

#ifdef HANDMADE_MATH__USE_SSE
HMM_INLINE hmm_mat4 HMM_SubtractMat4(hmm_mat4 Left, hmm_mat4 Right);

#else
HMM_EXTERN hmm_mat4 HMM_SubtractMat4(hmm_mat4 Left, hmm_mat4 Right);
#endif

HMM_EXTERN hmm_mat4 HMM_MultiplyMat4(hmm_mat4 Left, hmm_mat4 Right);

#ifdef HANDMADE_MATH__USE_SSE
HMM_INLINE hmm_mat4 HMM_MultiplyMat4f(hmm_mat4 Matrix, float Scalar);

#else
HMM_EXTERN hmm_mat4 HMM_MultiplyMat4f(hmm_mat4 Matrix, float Scalar);
#endif

HMM_EXTERN hmm_vec4 HMM_MultiplyMat4ByVec4(hmm_mat4 Matrix, hmm_vec4 Vector);

#ifdef HANDMADE_MATH__USE_SSE
HMM_INLINE hmm_mat4 HMM_DivideMat4f(hmm_mat4 Matrix, float Scalar);

#else
HMM_EXTERN hmm_mat4 HMM_DivideMat4f(hmm_mat4 Matrix, float Scalar);
#endif


/*
 * Common graphics transformations
 */

HMM_INLINE hmm_mat4 HMM_Orthographic(float Left, float Right, float Bottom, float Top, float Near, float Far);


HMM_INLINE hmm_mat4 HMM_Perspective(float FOV, float AspectRatio, float Near, float Far);


HMM_INLINE hmm_mat4 HMM_Translate(const hmm_vec3& Translation);


HMM_EXTERN hmm_mat4 HMM_Rotate(float Angle,const hmm_vec3& Axis);

HMM_INLINE hmm_mat4 HMM_Scale(const hmm_vec3& Scale);


HMM_EXTERN hmm_mat4 HMM_LookAt(const hmm_vec3& Eye,const hmm_vec3& Center,const hmm_vec3& Up);


/*
 * Quaternion operations
 */

HMM_INLINE hmm_quaternion HMM_Quaternion(float X, float Y, float Z, float W);


HMM_INLINE hmm_quaternion HMM_QuaternionV4(hmm_vec4 Vector);


HMM_INLINE hmm_quaternion HMM_AddQuaternion(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_quaternion HMM_SubtractQuaternion(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_quaternion HMM_MultiplyQuaternion(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_quaternion HMM_MultiplyQuaternionF(hmm_quaternion Left, float Multiplicative);


HMM_INLINE hmm_quaternion HMM_DivideQuaternionF(hmm_quaternion Left, float Dividend);


HMM_EXTERN hmm_quaternion HMM_InverseQuaternion(hmm_quaternion Left);

HMM_INLINE float HMM_DotQuaternion(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_quaternion HMM_NormalizeQuaternion(hmm_quaternion Left);


HMM_INLINE hmm_quaternion HMM_NLerp(hmm_quaternion Left, float Time, hmm_quaternion Right);


HMM_EXTERN hmm_quaternion HMM_Slerp(hmm_quaternion Left, float Time, hmm_quaternion Right);
HMM_EXTERN hmm_mat4 HMM_QuaternionToMat4(hmm_quaternion Left);
HMM_EXTERN hmm_quaternion HMM_Mat4ToQuaternion(hmm_mat4 Left);
HMM_EXTERN hmm_quaternion HMM_QuaternionFromAxisAngle(const hmm_vec3& Axis, float AngleOfRotation);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

HMM_INLINE float HMM_Length(hmm_vec2 A);


HMM_INLINE float HMM_Length(const hmm_vec3& A);


HMM_INLINE float HMM_Length(hmm_vec4 A);


HMM_INLINE float HMM_LengthSquared(hmm_vec2 A);


HMM_INLINE float HMM_LengthSquared(const hmm_vec3& A);


HMM_INLINE float HMM_LengthSquared(hmm_vec4 A);


HMM_INLINE hmm_vec2 HMM_Normalize(hmm_vec2 A);


HMM_INLINE hmm_vec3 HMM_Normalize(const hmm_vec3& A);


HMM_INLINE hmm_vec4 HMM_Normalize(hmm_vec4 A);


HMM_INLINE hmm_vec2 HMM_FastNormalize(hmm_vec2 A);


HMM_INLINE hmm_vec3 HMM_FastNormalize(const hmm_vec3& A);


HMM_INLINE hmm_vec4 HMM_FastNormalize(hmm_vec4 A);


HMM_INLINE hmm_quaternion HMM_Normalize(hmm_quaternion A);


HMM_INLINE float HMM_Dot(hmm_vec2 VecOne, hmm_vec2 VecTwo);


HMM_INLINE float HMM_Dot(const hmm_vec3& VecOne,const hmm_vec3& VecTwo);


HMM_INLINE float HMM_Dot(hmm_vec4 VecOne, hmm_vec4 VecTwo);


HMM_INLINE float HMM_Dot(hmm_quaternion QuatOne, hmm_quaternion QuatTwo);


HMM_INLINE hmm_vec2 HMM_Add(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 HMM_Add(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 HMM_Add(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_mat4 HMM_Add(hmm_mat4 Left, hmm_mat4 Right);


HMM_INLINE hmm_quaternion HMM_Add(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_vec2 HMM_Subtract(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 HMM_Subtract(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 HMM_Subtract(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_mat4 HMM_Subtract(hmm_mat4 Left, hmm_mat4 Right);


HMM_INLINE hmm_quaternion HMM_Subtract(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_vec2 HMM_Multiply(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec2 HMM_Multiply(hmm_vec2 Left, float Right);


HMM_INLINE hmm_vec3 HMM_Multiply(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec3 HMM_Multiply(const hmm_vec3& Left, float Right);


HMM_INLINE hmm_vec4 HMM_Multiply(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_vec4 HMM_Multiply(hmm_vec4 Left, float Right);


HMM_INLINE hmm_mat4 HMM_Multiply(hmm_mat4 Left, hmm_mat4 Right);


HMM_INLINE hmm_mat4 HMM_Multiply(hmm_mat4 Left, float Right);


HMM_INLINE hmm_vec4 HMM_Multiply(hmm_mat4 Matrix, hmm_vec4 Vector);


HMM_INLINE hmm_quaternion HMM_Multiply(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_quaternion HMM_Multiply(hmm_quaternion Left, float Right);


HMM_INLINE hmm_vec2 HMM_Divide(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec2 HMM_Divide(hmm_vec2 Left, float Right);


HMM_INLINE hmm_vec3 HMM_Divide(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec3 HMM_Divide(const hmm_vec3& Left, float Right);


HMM_INLINE hmm_vec4 HMM_Divide(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_vec4 HMM_Divide(hmm_vec4 Left, float Right);


HMM_INLINE hmm_mat4 HMM_Divide(hmm_mat4 Left, float Right);


HMM_INLINE hmm_quaternion HMM_Divide(hmm_quaternion Left, float Right);


HMM_INLINE hmm_bool HMM_Equals(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_bool HMM_Equals(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_bool HMM_Equals(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_vec2 operator+(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 operator-(const hmm_vec3& Left );

HMM_INLINE hmm_vec3 operator+(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 operator+(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_mat4 operator+(hmm_mat4 Left, hmm_mat4 Right);


HMM_INLINE hmm_quaternion operator+(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_vec2 operator-(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 operator-(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 operator-(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_mat4 operator-(hmm_mat4 Left, hmm_mat4 Right);


HMM_INLINE hmm_quaternion operator-(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_vec2 operator*(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 operator*(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 operator*(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_mat4 operator*(hmm_mat4 Left, hmm_mat4 Right);


HMM_INLINE hmm_quaternion operator*(hmm_quaternion Left, hmm_quaternion Right);


HMM_INLINE hmm_vec2 operator*(hmm_vec2 Left, float Right);


HMM_INLINE hmm_vec3 operator*(const hmm_vec3& Left, float Right);


HMM_INLINE hmm_vec4 operator*(hmm_vec4 Left, float Right);


HMM_INLINE hmm_mat4 operator*(hmm_mat4 Left, float Right);


HMM_INLINE hmm_quaternion operator*(hmm_quaternion Left, float Right);


HMM_INLINE hmm_vec2 operator*(float Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 operator*(float Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 operator*(float Left, hmm_vec4 Right);


HMM_INLINE hmm_mat4 operator*(float Left, hmm_mat4 Right);


HMM_INLINE hmm_quaternion operator*(float Left, hmm_quaternion Right);


HMM_INLINE hmm_vec4 operator*(hmm_mat4 Matrix, hmm_vec4 Vector);


HMM_INLINE hmm_vec2 operator/(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 operator/(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 operator/(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_vec2 operator/(hmm_vec2 Left, float Right);


HMM_INLINE hmm_vec3 operator/(const hmm_vec3& Left, float Right);


HMM_INLINE hmm_vec4 operator/(hmm_vec4 Left, float Right);


HMM_INLINE hmm_mat4 operator/(hmm_mat4 Left, float Right);


HMM_INLINE hmm_quaternion operator/(hmm_quaternion Left, float Right);


HMM_INLINE hmm_vec2 &operator+=(hmm_vec2 &Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 &operator+=(hmm_vec3 &Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 &operator+=(hmm_vec4 &Left, hmm_vec4 Right);


HMM_INLINE hmm_mat4 &operator+=(hmm_mat4 &Left, hmm_mat4 Right);


HMM_INLINE hmm_quaternion &operator+=(hmm_quaternion &Left, hmm_quaternion Right);


HMM_INLINE hmm_vec2 &operator-=(hmm_vec2 &Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 &operator-=(hmm_vec3 &Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 &operator-=(hmm_vec4 &Left, hmm_vec4 Right);


HMM_INLINE hmm_mat4 &operator-=(hmm_mat4 &Left, hmm_mat4 Right);


HMM_INLINE hmm_quaternion &operator-=(hmm_quaternion &Left, hmm_quaternion Right);


HMM_INLINE hmm_vec2 &operator*=(hmm_vec2 &Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 &operator*=(hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 &operator*=(hmm_vec4 &Left, hmm_vec4 Right);


HMM_INLINE hmm_vec2 &operator*=(hmm_vec2 &Left, float Right);


HMM_INLINE hmm_vec3 &operator*=(hmm_vec3& Left, float Right);


HMM_INLINE hmm_vec4 &operator*=(hmm_vec4 &Left, float Right);


HMM_INLINE hmm_mat4 &operator*=(hmm_mat4 &Left, float Right);


HMM_INLINE hmm_quaternion &operator*=(hmm_quaternion &Left, float Right);


HMM_INLINE hmm_vec2 &operator/=(hmm_vec2 &Left, hmm_vec2 Right);


HMM_INLINE hmm_vec3 &operator/=(hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_vec4 &operator/=(hmm_vec4 &Left, hmm_vec4 Right);


HMM_INLINE hmm_vec2 &operator/=(hmm_vec2 &Left, float Right);


HMM_INLINE hmm_vec3 &operator/=(hmm_vec3& Left, float Right);


HMM_INLINE hmm_vec4 &operator/=(hmm_vec4 &Left, float Right);


HMM_INLINE hmm_mat4 &operator/=(hmm_mat4 &Left, float Right);


HMM_INLINE hmm_quaternion &operator/=(hmm_quaternion &Left, float Right);


HMM_INLINE hmm_bool operator==(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_bool operator==(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_bool operator==(hmm_vec4 Left, hmm_vec4 Right);


HMM_INLINE hmm_bool operator!=(hmm_vec2 Left, hmm_vec2 Right);


HMM_INLINE hmm_bool operator!=(const hmm_vec3& Left,const hmm_vec3& Right);


HMM_INLINE hmm_bool operator!=(hmm_vec4 Left, hmm_vec4 Right);


#endif /* __cplusplus */

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#if defined(__GNUC__) && (__GNUC__ == 4 && __GNUC_MINOR__ < 8)
#pragma GCC diagnostic pop
#endif

#endif /* HANDMADE_MATH_H */

#ifdef HANDMADE_MATH_IMPLEMENTATION

float HMM_Power(float Base, int Exponent);


#ifndef HANDMADE_MATH__USE_SSE
hmm_mat4 HMM_Transpose(hmm_mat4 Matrix);

#endif

#ifndef HANDMADE_MATH__USE_SSE
hmm_mat4 HMM_AddMat4(hmm_mat4 Left, hmm_mat4 Right);

#endif

#ifndef HANDMADE_MATH__USE_SSE
hmm_mat4 HMM_SubtractMat4(hmm_mat4 Left, hmm_mat4 Right);

#endif

hmm_mat4 HMM_MultiplyMat4(hmm_mat4 Left, hmm_mat4 Right);


#ifndef HANDMADE_MATH__USE_SSE
hmm_mat4 HMM_MultiplyMat4f(hmm_mat4 Matrix, float Scalar);

#endif

hmm_vec4 HMM_MultiplyMat4ByVec4(hmm_mat4 Matrix, hmm_vec4 Vector);


#ifndef HANDMADE_MATH__USE_SSE
hmm_mat4 HMM_DivideMat4f(hmm_mat4 Matrix, float Scalar);

#endif

hmm_mat4 HMM_Rotate(float Angle,hmm_vec3 Axis);


hmm_mat4 HMM_LookAt(const hmm_vec3& Eye,const hmm_vec3& Center,const hmm_vec3& Up);


hmm_quaternion HMM_InverseQuaternion(hmm_quaternion Left);


hmm_quaternion HMM_Slerp(hmm_quaternion Left, float Time, hmm_quaternion Right);


hmm_mat4 HMM_QuaternionToMat4(hmm_quaternion Left);


// This method taken from Mike Day at Insomniac Games.
// https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
//
// Note that as mentioned at the top of the paper, the paper assumes the matrix
// would be *post*-multiplied to a vector to rotate it, meaning the matrix is
// the transpose of what we're dealing with. But, because our matrices are
// stored in column-major order, the indices *appear* to match the paper.
//
// For example, m12 in the paper is row 1, column 2. We need to transpose it to
// row 2, column 1. But, because the column comes first when referencing
// elements, it looks like M.Elements[1][2].
//
// Don't be confused! Or if you must be confused, at least trust this
// comment. :);
hmm_quaternion HMM_Mat4ToQuaternion(hmm_mat4 M);


hmm_quaternion HMM_QuaternionFromAxisAngle(const hmm_vec3& Axis, float AngleOfRotation);


HMM_INLINE hmm_vec3 HMM_Reflect(const hmm_vec3& v ,const hmm_vec3& n );


// Matrix inversion code taken from
// https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html



#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

// vec(0, 1, 2, 3) -> (vec[x], vec[y], vec[z], vec[w]);
#define VecSwizzleMask(vec, mask)          _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), mask))
#define VecSwizzle(vec, x, y, z, w)        VecSwizzleMask(vec, MakeShuffleMask(x,y,z,w))
#define VecSwizzle1(vec, x)                VecSwizzleMask(vec, MakeShuffleMask(x,x,x,x))
// special swizzle
#define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
#define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)

// return (vec1[x], vec1[y], vec2[z], vec2[w]);
#define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
// special shuffle
#define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
#define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)



// for column major matrix
// we use __m128 to represent 2x2 matrix as A = | A0  A2 |
//                                              | A1  A3 |
// 2x2 column major Matrix multiply A*B
inline __m128 Mat2Mul(__m128 vec1, __m128 vec2);

// 2x2 column major Matrix adjugate multiply (A#)*B
__m128 Mat2AdjMul(__m128 vec1, __m128 vec2);

// 2x2 column major Matrix multiply adjugate A*(B#);
__m128 Mat2MulAdj(__m128 vec1, __m128 vec2);


// Inverse function is the same no matter column major or row major
// this version treats it as column major
inline hmm_mat4 GetInverse(const hmm_mat4 & inM);


hmm_vec3 HMM_UnProject(
    const hmm_vec3 &pos,
    const hmm_mat4 &vp, 
    int width, int height );

typedef hmm_vec2 v2;
typedef hmm_vec3 v3;
typedef hmm_vec4 v4;
typedef hmm_mat4 m4;
typedef hmm_quaternion q4;
#define HMM_RADIANS( x ) ( ( HMM_PI / 180 ) * (x) )



void print_v3( const v3 &v ){
  fprintf( stdout,"%f, %f, %f",v.X,v.Y,v.Z );
}


#endif /* HANDMADE_MATH_IMPLEMENTATION */
#endif
