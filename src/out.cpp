#include "HandmadeMath.h"
COVERAGE(HMM_SinF, 1)
HMM_INLINE float HMM_SinF(float Radians)


COVERAGE(HMM_CosF, 1)
HMM_INLINE float HMM_CosF(float Radians)


COVERAGE(HMM_TanF, 1)
HMM_INLINE float HMM_TanF(float Radians)


COVERAGE(HMM_ACosF, 1)
HMM_INLINE float HMM_ACosF(float Radians)


COVERAGE(HMM_ATanF, 1)
HMM_INLINE float HMM_ATanF(float Radians)


COVERAGE(HMM_ATan2F, 1)
HMM_INLINE float HMM_ATan2F(float Left, float Right)


COVERAGE(HMM_ExpF, 1)
HMM_INLINE float HMM_ExpF(float Float)


COVERAGE(HMM_LogF, 1)
HMM_INLINE float HMM_LogF(float Float)


COVERAGE(HMM_SquareRootF, 1)
HMM_INLINE float HMM_SquareRootF(float Float)


COVERAGE(HMM_RSquareRootF, 1)
HMM_INLINE float HMM_RSquareRootF(float Float)


HMM_EXTERN float HMM_Power(float Base, int Exponent);

COVERAGE(HMM_PowerF, 1)
HMM_INLINE float HMM_PowerF(float Base, float Exponent)



/*
 * Utility functions
 */

COVERAGE(HMM_ToRadians, 1)
HMM_INLINE float HMM_ToRadians(float Degrees)


COVERAGE(HMM_Lerp, 1)
HMM_INLINE float HMM_Lerp(float A, float Time, float B)


COVERAGE(HMM_Clamp, 1)
HMM_INLINE float HMM_Clamp(float Min, float Value, float Max)



/*
 * Vector initialization
 */

COVERAGE(HMM_Vec2, 1)
HMM_INLINE hmm_vec2 HMM_Vec2(float X, float Y)


COVERAGE(HMM_Vec2i, 1)
HMM_INLINE hmm_vec2 HMM_Vec2i(int X, int Y)


COVERAGE(HMM_Vec3, 1)
HMM_INLINE hmm_vec3 HMM_Vec3(float X, float Y, float Z)


COVERAGE(HMM_Vec3i, 1)
HMM_INLINE hmm_vec3 HMM_Vec3i(int X, int Y, int Z)


COVERAGE(HMM_Vec4, 1)
HMM_INLINE hmm_vec4 HMM_Vec4(float X, float Y, float Z, float W)


COVERAGE(HMM_Vec4i, 1)
HMM_INLINE hmm_vec4 HMM_Vec4i(int X, int Y, int Z, int W)


COVERAGE(HMM_Vec4v, 1)
HMM_INLINE hmm_vec4 HMM_Vec4v(const hmm_vec3& Vector, float W)



/*
 * Binary vector operations
 */
COVERAGE(HMM_AddVec2, 1)
HMM_INLINE hmm_vec2 HMM_AddVec2(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_AddVec3, 1)
HMM_INLINE hmm_vec3 HMM_AddVec3(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_AddVec4, 1)
HMM_INLINE hmm_vec4 HMM_AddVec4(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_SubtractVec2, 1)
HMM_INLINE hmm_vec2 HMM_SubtractVec2(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_SubtractVec3, 1)
HMM_INLINE hmm_vec3 HMM_SubtractVec3(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_SubtractVec4, 1)
HMM_INLINE hmm_vec4 HMM_SubtractVec4(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_MultiplyVec2, 1)
HMM_INLINE hmm_vec2 HMM_MultiplyVec2(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_MultiplyVec2f, 1)
HMM_INLINE hmm_vec2 HMM_MultiplyVec2f(hmm_vec2 Left, float Right)


COVERAGE(HMM_MultiplyVec3, 1)
HMM_INLINE hmm_vec3 HMM_MultiplyVec3(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_MultiplyVec3f, 1)
HMM_INLINE hmm_vec3 HMM_MultiplyVec3f(const hmm_vec3& Left, float Right)


COVERAGE(HMM_MultiplyVec4, 1)
HMM_INLINE hmm_vec4 HMM_MultiplyVec4(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_MultiplyVec4f, 1)
HMM_INLINE hmm_vec4 HMM_MultiplyVec4f(hmm_vec4 Left, float Right)


COVERAGE(HMM_DivideVec2, 1)
HMM_INLINE hmm_vec2 HMM_DivideVec2(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_DivideVec2f, 1)
HMM_INLINE hmm_vec2 HMM_DivideVec2f(hmm_vec2 Left, float Right)


COVERAGE(HMM_DivideVec3, 1)
HMM_INLINE hmm_vec3 HMM_DivideVec3(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_DivideVec3f, 1)
HMM_INLINE hmm_vec3 HMM_DivideVec3f(const hmm_vec3& Left, float Right)


COVERAGE(HMM_DivideVec4, 1)
HMM_INLINE hmm_vec4 HMM_DivideVec4(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_DivideVec4f, 1)
HMM_INLINE hmm_vec4 HMM_DivideVec4f(hmm_vec4 Left, float Right)


COVERAGE(HMM_EqualsVec2, 1)
HMM_INLINE hmm_bool HMM_EqualsVec2(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_EqualsVec3, 1)
HMM_INLINE hmm_bool HMM_EqualsVec3(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_EqualsVec4, 1)
HMM_INLINE hmm_bool HMM_EqualsVec4(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_DotVec2, 1)
HMM_INLINE float HMM_DotVec2(hmm_vec2 VecOne, hmm_vec2 VecTwo)


COVERAGE(HMM_DotVec3, 1)
HMM_INLINE float HMM_DotVec3(const hmm_vec3& VecOne,const hmm_vec3& VecTwo)


COVERAGE(HMM_DotVec4, 1)
HMM_INLINE float HMM_DotVec4(hmm_vec4 VecOne, hmm_vec4 VecTwo)


COVERAGE(HMM_Cross, 1)
HMM_INLINE hmm_vec3 HMM_Cross(const hmm_vec3& VecOne,const hmm_vec3& VecTwo)



/*
 * Unary vector operations
 */

COVERAGE(HMM_LengthSquaredVec2, 1)
HMM_INLINE float HMM_LengthSquaredVec2(hmm_vec2 A)


COVERAGE(HMM_LengthSquaredVec3, 1)
HMM_INLINE float HMM_LengthSquaredVec3(const hmm_vec3& A)


COVERAGE(HMM_LengthSquaredVec4, 1)
HMM_INLINE float HMM_LengthSquaredVec4(hmm_vec4 A)


COVERAGE(HMM_LengthVec2, 1)
HMM_INLINE float HMM_LengthVec2(hmm_vec2 A)


COVERAGE(HMM_LengthVec3, 1)
HMM_INLINE float HMM_LengthVec3(const hmm_vec3& A)


COVERAGE(HMM_LengthVec4, 1)
HMM_INLINE float HMM_LengthVec4(hmm_vec4 A)


COVERAGE(HMM_NormalizeVec2, 2)
HMM_INLINE hmm_vec2 HMM_NormalizeVec2(hmm_vec2 A)


COVERAGE(HMM_NormalizeVec3, 2)
HMM_INLINE hmm_vec3 HMM_NormalizeVec3(const hmm_vec3& A)


COVERAGE(HMM_NormalizeVec4, 2)
HMM_INLINE hmm_vec4 HMM_NormalizeVec4(hmm_vec4 A)


COVERAGE(HMM_FastNormalizeVec2, 1)
HMM_INLINE hmm_vec2 HMM_FastNormalizeVec2(hmm_vec2 A)


COVERAGE(HMM_FastNormalizeVec3, 1)
HMM_INLINE hmm_vec3 HMM_FastNormalizeVec3(const hmm_vec3& A)


COVERAGE(HMM_FastNormalizeVec4, 1)
HMM_INLINE hmm_vec4 HMM_FastNormalizeVec4(hmm_vec4 A)



/*
 * SSE stuff
 */

#ifdef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_LinearCombineSSE, 1)
HMM_INLINE __m128 HMM_LinearCombineSSE(__m128 Left, hmm_mat4 Right)

#endif


/*
 * Matrix functions
 */

COVERAGE(HMM_Mat4, 1)
HMM_INLINE hmm_mat4 HMM_Mat4(void)


COVERAGE(HMM_Mat4d, 1)
HMM_INLINE hmm_mat4 HMM_Mat4d(float Diagonal)


#ifdef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_Transpose, 1)
HMM_INLINE hmm_mat4 HMM_Transpose(hmm_mat4 Matrix)

#else
HMM_EXTERN hmm_mat4 HMM_Transpose(hmm_mat4 Matrix);
#endif

#ifdef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_AddMat4, 1)
HMM_INLINE hmm_mat4 HMM_AddMat4(hmm_mat4 Left, hmm_mat4 Right)

#else
HMM_EXTERN hmm_mat4 HMM_AddMat4(hmm_mat4 Left, hmm_mat4 Right);
#endif

#ifdef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_SubtractMat4, 1)
HMM_INLINE hmm_mat4 HMM_SubtractMat4(hmm_mat4 Left, hmm_mat4 Right)

#else
HMM_EXTERN hmm_mat4 HMM_SubtractMat4(hmm_mat4 Left, hmm_mat4 Right);
#endif

HMM_EXTERN hmm_mat4 HMM_MultiplyMat4(hmm_mat4 Left, hmm_mat4 Right);

#ifdef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_MultiplyMat4f, 1)
HMM_INLINE hmm_mat4 HMM_MultiplyMat4f(hmm_mat4 Matrix, float Scalar)

#else
HMM_EXTERN hmm_mat4 HMM_MultiplyMat4f(hmm_mat4 Matrix, float Scalar);
#endif

HMM_EXTERN hmm_vec4 HMM_MultiplyMat4ByVec4(hmm_mat4 Matrix, hmm_vec4 Vector);

#ifdef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_DivideMat4f, 1)
HMM_INLINE hmm_mat4 HMM_DivideMat4f(hmm_mat4 Matrix, float Scalar)

#else
HMM_EXTERN hmm_mat4 HMM_DivideMat4f(hmm_mat4 Matrix, float Scalar);
#endif


/*
 * Common graphics transformations
 */

COVERAGE(HMM_Orthographic, 1)
HMM_INLINE hmm_mat4 HMM_Orthographic(float Left, float Right, float Bottom, float Top, float Near, float Far)


COVERAGE(HMM_Perspective, 1)
HMM_INLINE hmm_mat4 HMM_Perspective(float FOV, float AspectRatio, float Near, float Far)


COVERAGE(HMM_Translate, 1)
HMM_INLINE hmm_mat4 HMM_Translate(const hmm_vec3& Translation)


HMM_EXTERN hmm_mat4 HMM_Rotate(float Angle,const hmm_vec3& Axis);

COVERAGE(HMM_Scale, 1)
HMM_INLINE hmm_mat4 HMM_Scale(const hmm_vec3& Scale)


HMM_EXTERN hmm_mat4 HMM_LookAt(const hmm_vec3& Eye,const hmm_vec3& Center,const hmm_vec3& Up);


/*
 * Quaternion operations
 */

COVERAGE(HMM_Quaternion, 1)
HMM_INLINE hmm_quaternion HMM_Quaternion(float X, float Y, float Z, float W)


COVERAGE(HMM_QuaternionV4, 1)
HMM_INLINE hmm_quaternion HMM_QuaternionV4(hmm_vec4 Vector)


COVERAGE(HMM_AddQuaternion, 1)
HMM_INLINE hmm_quaternion HMM_AddQuaternion(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_SubtractQuaternion, 1)
HMM_INLINE hmm_quaternion HMM_SubtractQuaternion(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_MultiplyQuaternion, 1)
HMM_INLINE hmm_quaternion HMM_MultiplyQuaternion(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_MultiplyQuaternionF, 1)
HMM_INLINE hmm_quaternion HMM_MultiplyQuaternionF(hmm_quaternion Left, float Multiplicative)


COVERAGE(HMM_DivideQuaternionF, 1)
HMM_INLINE hmm_quaternion HMM_DivideQuaternionF(hmm_quaternion Left, float Dividend)


HMM_EXTERN hmm_quaternion HMM_InverseQuaternion(hmm_quaternion Left);

COVERAGE(HMM_DotQuaternion, 1)
HMM_INLINE float HMM_DotQuaternion(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_NormalizeQuaternion, 1)
HMM_INLINE hmm_quaternion HMM_NormalizeQuaternion(hmm_quaternion Left)


COVERAGE(HMM_NLerp, 1)
HMM_INLINE hmm_quaternion HMM_NLerp(hmm_quaternion Left, float Time, hmm_quaternion Right)


HMM_EXTERN hmm_quaternion HMM_Slerp(hmm_quaternion Left, float Time, hmm_quaternion Right);
HMM_EXTERN hmm_mat4 HMM_QuaternionToMat4(hmm_quaternion Left);
HMM_EXTERN hmm_quaternion HMM_Mat4ToQuaternion(hmm_mat4 Left);
HMM_EXTERN hmm_quaternion HMM_QuaternionFromAxisAngle(const hmm_vec3& Axis, float AngleOfRotation);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

COVERAGE(HMM_LengthVec2CPP, 1)
HMM_INLINE float HMM_Length(hmm_vec2 A)


COVERAGE(HMM_LengthVec3CPP, 1)
HMM_INLINE float HMM_Length(const hmm_vec3& A)


COVERAGE(HMM_LengthVec4CPP, 1)
HMM_INLINE float HMM_Length(hmm_vec4 A)


COVERAGE(HMM_LengthSquaredVec2CPP, 1)
HMM_INLINE float HMM_LengthSquared(hmm_vec2 A)


COVERAGE(HMM_LengthSquaredVec3CPP, 1)
HMM_INLINE float HMM_LengthSquared(const hmm_vec3& A)


COVERAGE(HMM_LengthSquaredVec4CPP, 1)
HMM_INLINE float HMM_LengthSquared(hmm_vec4 A)


COVERAGE(HMM_NormalizeVec2CPP, 1)
HMM_INLINE hmm_vec2 HMM_Normalize(hmm_vec2 A)


COVERAGE(HMM_NormalizeVec3CPP, 1)
HMM_INLINE hmm_vec3 HMM_Normalize(const hmm_vec3& A)


COVERAGE(HMM_NormalizeVec4CPP, 1)
HMM_INLINE hmm_vec4 HMM_Normalize(hmm_vec4 A)


COVERAGE(HMM_FastNormalizeVec2CPP, 1)
HMM_INLINE hmm_vec2 HMM_FastNormalize(hmm_vec2 A)


COVERAGE(HMM_FastNormalizeVec3CPP, 1)
HMM_INLINE hmm_vec3 HMM_FastNormalize(const hmm_vec3& A)


COVERAGE(HMM_FastNormalizeVec4CPP, 1)
HMM_INLINE hmm_vec4 HMM_FastNormalize(hmm_vec4 A)


COVERAGE(HMM_NormalizeQuaternionCPP, 1)
HMM_INLINE hmm_quaternion HMM_Normalize(hmm_quaternion A)


COVERAGE(HMM_DotVec2CPP, 1)
HMM_INLINE float HMM_Dot(hmm_vec2 VecOne, hmm_vec2 VecTwo)


COVERAGE(HMM_DotVec3CPP, 1)
HMM_INLINE float HMM_Dot(const hmm_vec3& VecOne,const hmm_vec3& VecTwo)


COVERAGE(HMM_DotVec4CPP, 1)
HMM_INLINE float HMM_Dot(hmm_vec4 VecOne, hmm_vec4 VecTwo)


COVERAGE(HMM_DotQuaternionCPP, 1)
HMM_INLINE float HMM_Dot(hmm_quaternion QuatOne, hmm_quaternion QuatTwo)


COVERAGE(HMM_AddVec2CPP, 1)
HMM_INLINE hmm_vec2 HMM_Add(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_AddVec3CPP, 1)
HMM_INLINE hmm_vec3 HMM_Add(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_AddVec4CPP, 1)
HMM_INLINE hmm_vec4 HMM_Add(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_AddMat4CPP, 1)
HMM_INLINE hmm_mat4 HMM_Add(hmm_mat4 Left, hmm_mat4 Right)


COVERAGE(HMM_AddQuaternionCPP, 1)
HMM_INLINE hmm_quaternion HMM_Add(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_SubtractVec2CPP, 1)
HMM_INLINE hmm_vec2 HMM_Subtract(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_SubtractVec3CPP, 1)
HMM_INLINE hmm_vec3 HMM_Subtract(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_SubtractVec4CPP, 1)
HMM_INLINE hmm_vec4 HMM_Subtract(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_SubtractMat4CPP, 1)
HMM_INLINE hmm_mat4 HMM_Subtract(hmm_mat4 Left, hmm_mat4 Right)


COVERAGE(HMM_SubtractQuaternionCPP, 1)
HMM_INLINE hmm_quaternion HMM_Subtract(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_MultiplyVec2CPP, 1)
HMM_INLINE hmm_vec2 HMM_Multiply(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_MultiplyVec2fCPP, 1)
HMM_INLINE hmm_vec2 HMM_Multiply(hmm_vec2 Left, float Right)


COVERAGE(HMM_MultiplyVec3CPP, 1)
HMM_INLINE hmm_vec3 HMM_Multiply(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_MultiplyVec3fCPP, 1)
HMM_INLINE hmm_vec3 HMM_Multiply(const hmm_vec3& Left, float Right)


COVERAGE(HMM_MultiplyVec4CPP, 1)
HMM_INLINE hmm_vec4 HMM_Multiply(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_MultiplyVec4fCPP, 1)
HMM_INLINE hmm_vec4 HMM_Multiply(hmm_vec4 Left, float Right)


COVERAGE(HMM_MultiplyMat4CPP, 1)
HMM_INLINE hmm_mat4 HMM_Multiply(hmm_mat4 Left, hmm_mat4 Right)


COVERAGE(HMM_MultiplyMat4fCPP, 1)
HMM_INLINE hmm_mat4 HMM_Multiply(hmm_mat4 Left, float Right)


COVERAGE(HMM_MultiplyMat4ByVec4CPP, 1)
HMM_INLINE hmm_vec4 HMM_Multiply(hmm_mat4 Matrix, hmm_vec4 Vector)


COVERAGE(HMM_MultiplyQuaternionCPP, 1)
HMM_INLINE hmm_quaternion HMM_Multiply(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_MultiplyQuaternionFCPP, 1)
HMM_INLINE hmm_quaternion HMM_Multiply(hmm_quaternion Left, float Right)


COVERAGE(HMM_DivideVec2CPP, 1)
HMM_INLINE hmm_vec2 HMM_Divide(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_DivideVec2fCPP, 1)
HMM_INLINE hmm_vec2 HMM_Divide(hmm_vec2 Left, float Right)


COVERAGE(HMM_DivideVec3CPP, 1)
HMM_INLINE hmm_vec3 HMM_Divide(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_DivideVec3fCPP, 1)
HMM_INLINE hmm_vec3 HMM_Divide(const hmm_vec3& Left, float Right)


COVERAGE(HMM_DivideVec4CPP, 1)
HMM_INLINE hmm_vec4 HMM_Divide(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_DivideVec4fCPP, 1)
HMM_INLINE hmm_vec4 HMM_Divide(hmm_vec4 Left, float Right)


COVERAGE(HMM_DivideMat4fCPP, 1)
HMM_INLINE hmm_mat4 HMM_Divide(hmm_mat4 Left, float Right)


COVERAGE(HMM_DivideQuaternionFCPP, 1)
HMM_INLINE hmm_quaternion HMM_Divide(hmm_quaternion Left, float Right)


COVERAGE(HMM_EqualsVec2CPP, 1)
HMM_INLINE hmm_bool HMM_Equals(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_EqualsVec3CPP, 1)
HMM_INLINE hmm_bool HMM_Equals(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_EqualsVec4CPP, 1)
HMM_INLINE hmm_bool HMM_Equals(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_AddVec2Op, 1)
HMM_INLINE hmm_vec2 operator+(hmm_vec2 Left, hmm_vec2 Right)


HMM_INLINE hmm_vec3 operator-(const hmm_vec3& Left )

COVERAGE(HMM_AddVec3Op, 1)
HMM_INLINE hmm_vec3 operator+(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_AddVec4Op, 1)
HMM_INLINE hmm_vec4 operator+(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_AddMat4Op, 1)
HMM_INLINE hmm_mat4 operator+(hmm_mat4 Left, hmm_mat4 Right)


COVERAGE(HMM_AddQuaternionOp, 1)
HMM_INLINE hmm_quaternion operator+(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_SubtractVec2Op, 1)
HMM_INLINE hmm_vec2 operator-(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_SubtractVec3Op, 1)
HMM_INLINE hmm_vec3 operator-(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_SubtractVec4Op, 1)
HMM_INLINE hmm_vec4 operator-(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_SubtractMat4Op, 1)
HMM_INLINE hmm_mat4 operator-(hmm_mat4 Left, hmm_mat4 Right)


COVERAGE(HMM_SubtractQuaternionOp, 1)
HMM_INLINE hmm_quaternion operator-(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_MultiplyVec2Op, 1)
HMM_INLINE hmm_vec2 operator*(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_MultiplyVec3Op, 1)
HMM_INLINE hmm_vec3 operator*(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_MultiplyVec4Op, 1)
HMM_INLINE hmm_vec4 operator*(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_MultiplyMat4Op, 1)
HMM_INLINE hmm_mat4 operator*(hmm_mat4 Left, hmm_mat4 Right)


COVERAGE(HMM_MultiplyQuaternionOp, 1)
HMM_INLINE hmm_quaternion operator*(hmm_quaternion Left, hmm_quaternion Right)


COVERAGE(HMM_MultiplyVec2fOp, 1)
HMM_INLINE hmm_vec2 operator*(hmm_vec2 Left, float Right)


COVERAGE(HMM_MultiplyVec3fOp, 1)
HMM_INLINE hmm_vec3 operator*(const hmm_vec3& Left, float Right)


COVERAGE(HMM_MultiplyVec4fOp, 1)
HMM_INLINE hmm_vec4 operator*(hmm_vec4 Left, float Right)


COVERAGE(HMM_MultiplyMat4fOp, 1)
HMM_INLINE hmm_mat4 operator*(hmm_mat4 Left, float Right)


COVERAGE(HMM_MultiplyQuaternionFOp, 1)
HMM_INLINE hmm_quaternion operator*(hmm_quaternion Left, float Right)


COVERAGE(HMM_MultiplyVec2fOpLeft, 1)
HMM_INLINE hmm_vec2 operator*(float Left, hmm_vec2 Right)


COVERAGE(HMM_MultiplyVec3fOpLeft, 1)
HMM_INLINE hmm_vec3 operator*(float Left,const hmm_vec3& Right)


COVERAGE(HMM_MultiplyVec4fOpLeft, 1)
HMM_INLINE hmm_vec4 operator*(float Left, hmm_vec4 Right)


COVERAGE(HMM_MultiplyMat4fOpLeft, 1)
HMM_INLINE hmm_mat4 operator*(float Left, hmm_mat4 Right)


COVERAGE(HMM_MultiplyQuaternionFOpLeft, 1)
HMM_INLINE hmm_quaternion operator*(float Left, hmm_quaternion Right)


COVERAGE(HMM_MultiplyMat4ByVec4Op, 1)
HMM_INLINE hmm_vec4 operator*(hmm_mat4 Matrix, hmm_vec4 Vector)


COVERAGE(HMM_DivideVec2Op, 1)
HMM_INLINE hmm_vec2 operator/(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_DivideVec3Op, 1)
HMM_INLINE hmm_vec3 operator/(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_DivideVec4Op, 1)
HMM_INLINE hmm_vec4 operator/(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_DivideVec2fOp, 1)
HMM_INLINE hmm_vec2 operator/(hmm_vec2 Left, float Right)


COVERAGE(HMM_DivideVec3fOp, 1)
HMM_INLINE hmm_vec3 operator/(const hmm_vec3& Left, float Right)


COVERAGE(HMM_DivideVec4fOp, 1)
HMM_INLINE hmm_vec4 operator/(hmm_vec4 Left, float Right)


COVERAGE(HMM_DivideMat4fOp, 1)
HMM_INLINE hmm_mat4 operator/(hmm_mat4 Left, float Right)


COVERAGE(HMM_DivideQuaternionFOp, 1)
HMM_INLINE hmm_quaternion operator/(hmm_quaternion Left, float Right)


COVERAGE(HMM_AddVec2Assign, 1)
HMM_INLINE hmm_vec2 &operator+=(hmm_vec2 &Left, hmm_vec2 Right)


COVERAGE(HMM_AddVec3Assign, 1)
HMM_INLINE hmm_vec3 &operator+=(hmm_vec3 &Left,const hmm_vec3& Right)


COVERAGE(HMM_AddVec4Assign, 1)
HMM_INLINE hmm_vec4 &operator+=(hmm_vec4 &Left, hmm_vec4 Right)


COVERAGE(HMM_AddMat4Assign, 1)
HMM_INLINE hmm_mat4 &operator+=(hmm_mat4 &Left, hmm_mat4 Right)


COVERAGE(HMM_AddQuaternionAssign, 1)
HMM_INLINE hmm_quaternion &operator+=(hmm_quaternion &Left, hmm_quaternion Right)


COVERAGE(HMM_SubtractVec2Assign, 1)
HMM_INLINE hmm_vec2 &operator-=(hmm_vec2 &Left, hmm_vec2 Right)


COVERAGE(HMM_SubtractVec3Assign, 1)
HMM_INLINE hmm_vec3 &operator-=(hmm_vec3 &Left,const hmm_vec3& Right)


COVERAGE(HMM_SubtractVec4Assign, 1)
HMM_INLINE hmm_vec4 &operator-=(hmm_vec4 &Left, hmm_vec4 Right)


COVERAGE(HMM_SubtractMat4Assign, 1)
HMM_INLINE hmm_mat4 &operator-=(hmm_mat4 &Left, hmm_mat4 Right)


COVERAGE(HMM_SubtractQuaternionAssign, 1)
HMM_INLINE hmm_quaternion &operator-=(hmm_quaternion &Left, hmm_quaternion Right)


COVERAGE(HMM_MultiplyVec2Assign, 1)
HMM_INLINE hmm_vec2 &operator*=(hmm_vec2 &Left, hmm_vec2 Right)


COVERAGE(HMM_MultiplyVec3Assign, 1)
HMM_INLINE hmm_vec3 &operator*=(hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_MultiplyVec4Assign, 1)
HMM_INLINE hmm_vec4 &operator*=(hmm_vec4 &Left, hmm_vec4 Right)


COVERAGE(HMM_MultiplyVec2fAssign, 1)
HMM_INLINE hmm_vec2 &operator*=(hmm_vec2 &Left, float Right)


COVERAGE(HMM_MultiplyVec3fAssign, 1)
HMM_INLINE hmm_vec3 &operator*=(hmm_vec3& Left, float Right)


COVERAGE(HMM_MultiplyVec4fAssign, 1)
HMM_INLINE hmm_vec4 &operator*=(hmm_vec4 &Left, float Right)


COVERAGE(HMM_MultiplyMat4fAssign, 1)
HMM_INLINE hmm_mat4 &operator*=(hmm_mat4 &Left, float Right)


COVERAGE(HMM_MultiplyQuaternionFAssign, 1)
HMM_INLINE hmm_quaternion &operator*=(hmm_quaternion &Left, float Right)


COVERAGE(HMM_DivideVec2Assign, 1)
HMM_INLINE hmm_vec2 &operator/=(hmm_vec2 &Left, hmm_vec2 Right)


COVERAGE(HMM_DivideVec3Assign, 1)
HMM_INLINE hmm_vec3 &operator/=(hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_DivideVec4Assign, 1)
HMM_INLINE hmm_vec4 &operator/=(hmm_vec4 &Left, hmm_vec4 Right)


COVERAGE(HMM_DivideVec2fAssign, 1)
HMM_INLINE hmm_vec2 &operator/=(hmm_vec2 &Left, float Right)


COVERAGE(HMM_DivideVec3fAssign, 1)
HMM_INLINE hmm_vec3 &operator/=(hmm_vec3& Left, float Right)


COVERAGE(HMM_DivideVec4fAssign, 1)
HMM_INLINE hmm_vec4 &operator/=(hmm_vec4 &Left, float Right)


COVERAGE(HMM_DivideMat4fAssign, 1)
HMM_INLINE hmm_mat4 &operator/=(hmm_mat4 &Left, float Right)


COVERAGE(HMM_DivideQuaternionFAssign, 1)
HMM_INLINE hmm_quaternion &operator/=(hmm_quaternion &Left, float Right)


COVERAGE(HMM_EqualsVec2Op, 1)
HMM_INLINE hmm_bool operator==(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_EqualsVec3Op, 1)
HMM_INLINE hmm_bool operator==(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_EqualsVec4Op, 1)
HMM_INLINE hmm_bool operator==(hmm_vec4 Left, hmm_vec4 Right)


COVERAGE(HMM_EqualsVec2OpNot, 1)
HMM_INLINE hmm_bool operator!=(hmm_vec2 Left, hmm_vec2 Right)


COVERAGE(HMM_EqualsVec3OpNot, 1)
HMM_INLINE hmm_bool operator!=(const hmm_vec3& Left,const hmm_vec3& Right)


COVERAGE(HMM_EqualsVec4OpNot, 1)
HMM_INLINE hmm_bool operator!=(hmm_vec4 Left, hmm_vec4 Right)


#endif /* __cplusplus */

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#if defined(__GNUC__) && (__GNUC__ == 4 && __GNUC_MINOR__ < 8)
#pragma GCC diagnostic pop
#endif

#endif /* HANDMADE_MATH_H */

#ifdef HANDMADE_MATH_IMPLEMENTATION

COVERAGE(HMM_Power, 2)
float HMM_Power(float Base, int Exponent)


#ifndef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_Transpose, 1)
hmm_mat4 HMM_Transpose(hmm_mat4 Matrix)

#endif

#ifndef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_AddMat4, 1)
hmm_mat4 HMM_AddMat4(hmm_mat4 Left, hmm_mat4 Right)

#endif

#ifndef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_SubtractMat4, 1)
hmm_mat4 HMM_SubtractMat4(hmm_mat4 Left, hmm_mat4 Right)

#endif

COVERAGE(HMM_MultiplyMat4, 1)
hmm_mat4 HMM_MultiplyMat4(hmm_mat4 Left, hmm_mat4 Right)


#ifndef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_MultiplyMat4f, 1)
hmm_mat4 HMM_MultiplyMat4f(hmm_mat4 Matrix, float Scalar)

#endif

COVERAGE(HMM_MultiplyMat4ByVec4, 1)
hmm_vec4 HMM_MultiplyMat4ByVec4(hmm_mat4 Matrix, hmm_vec4 Vector)


#ifndef HANDMADE_MATH__USE_SSE
COVERAGE(HMM_DivideMat4f, 1);
hmm_mat4 HMM_DivideMat4f(hmm_mat4 Matrix, float Scalar)

#endif

COVERAGE(HMM_Rotate, 1)
hmm_mat4 HMM_Rotate(float Angle,hmm_vec3 Axis)


COVERAGE(HMM_LookAt, 1)
hmm_mat4 HMM_LookAt(const hmm_vec3& Eye,const hmm_vec3& Center,const hmm_vec3& Up)


COVERAGE(HMM_InverseQuaternion, 1)
hmm_quaternion HMM_InverseQuaternion(hmm_quaternion Left)


COVERAGE(HMM_Slerp, 1)
hmm_quaternion HMM_Slerp(hmm_quaternion Left, float Time, hmm_quaternion Right)


COVERAGE(HMM_QuaternionToMat4, 1)
hmm_mat4 HMM_QuaternionToMat4(hmm_quaternion Left)


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
// comment. :)
COVERAGE(HMM_Mat4ToQuaternion, 4)
hmm_quaternion HMM_Mat4ToQuaternion(hmm_mat4 M)


COVERAGE(HMM_QuaternionFromAxisAngle, 1)
hmm_quaternion HMM_QuaternionFromAxisAngle(const hmm_vec3& Axis, float AngleOfRotation)


HMM_INLINE hmm_vec3 HMM_Reflect(const hmm_vec3& v ,const hmm_vec3& n )


// Matrix inversion code taken from
// https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html



#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

// vec(0, 1, 2, 3) -> (vec[x], vec[y], vec[z], vec[w])
#define VecSwizzleMask(vec, mask)          _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), mask))
#define VecSwizzle(vec, x, y, z, w)        VecSwizzleMask(vec, MakeShuffleMask(x,y,z,w))
#define VecSwizzle1(vec, x)                VecSwizzleMask(vec, MakeShuffleMask(x,x,x,x))
// special swizzle
#define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
#define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)

// return (vec1[x], vec1[y], vec2[z], vec2[w])
#define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
// special shuffle
#define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
#define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)



// for column major matrix
// we use __m128 to represent 2x2 matrix as A = | A0  A2 |
//                                              | A1  A3 |
// 2x2 column major Matrix multiply A*B
inline __m128 Mat2Mul(__m128 vec1, __m128 vec2)

// 2x2 column major Matrix adjugate multiply (A#)*B
__m128 Mat2AdjMul(__m128 vec1, __m128 vec2)

// 2x2 column major Matrix multiply adjugate A*(B#)
__m128 Mat2MulAdj(__m128 vec1, __m128 vec2)


// Inverse function is the same no matter column major or row major
// this version treats it as column major
inline hmm_mat4 GetInverse(const hmm_mat4 & inM)


hmm_vec3 HMM_UnProject(
    const hmm_vec3 &pos,
    const hmm_mat4 &vp, 
    int width, int height )


