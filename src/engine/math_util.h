#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <PR/ultratypes.h>

#include "types.h"

/* Scales the world down by this factor, increasing how far you can render on
 * console in exchange for a slight loss in precision.
 * 
 * For double extended boundary hacks, a value of 1.5f or 2.0f is good.
 * For quadruple extended bounds, use 3.f or 4.f
 */
#define WORLD_SCALE 2.f

/*
 * The sine and cosine tables overlap, but "#define gCosineTable (gSineTable +
 * 0x400)" doesn't give expected codegen; gSineTable and gCosineTable need to
 * be different symbols for code to match. Most likely the tables were placed
 * adjacent to each other, and gSineTable cut short, such that reads overflow
 * into gCosineTable.
 *
 * These kinds of out of bounds reads are undefined behavior, and break on
 * e.g. GCC (which doesn't place the tables next to each other, and probably
 * exploits array sizes for range analysis-based optimizations as well).
 * Thus, for non-IDO compilers we use the standard-compliant version.
 */
extern f32 gSineTable[];
#ifdef AVOID_UB
#define gCosineTable (gSineTable + 0x400)
#else
extern f32 gCosineTable[];
#endif

#define sins(x) gSineTable[(u16) (x) >> 4]
#define coss(x) gCosineTable[(u16) (x) >> 4]
#define tans(x) (sins(x) / coss(x))

#define min(a, b) ((a) <= (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

// from limits.h
#define S8_MAX __SCHAR_MAX__
#define S8_MIN (-S8_MAX - 1)
#define U8_MAX (S8_MAX * 2 + 1)
#define S16_MAX __SHRT_MAX__
#define S16_MIN (-S16_MAX - 1)
#define U16_MAX (S16_MAX * 2 + 1)
#define S32_MAX __INT_MAX__
#define S32_MIN (-S32_MAX - 1)
#define U32_MAX (S32_MAX * 2U + 1U)
#define S64_MAX __LONG_LONG_MAX__
#define S64_MIN (-S64_MAX - 1LL)
#define U64_MAX (S64_MAX * 2ULL + 1ULL)
#define F32_MAX __FLT_MAX__
#define F32_MIN __FLT_MIN__
#define F64_MAX __DBL_MAX__
#define F64_MIN __DBL_MIN__

#define CLAMP_U8( x)        CLAMP((x),     0x0,  U8_MAX)
#define CLAMP_S8( x)        CLAMP((x),  S8_MIN,  S8_MAX)
#define CLAMP_U16(x)        CLAMP((x),     0x0, U16_MAX)
#define CLAMP_S16(x)        CLAMP((x), S16_MIN, S16_MAX)
#define CLAMP_U32(x)        CLAMP((x),     0x0, U32_MAX)
#define CLAMP_S32(x)        CLAMP((x), S32_MIN, S32_MAX)
#define CLAMP_U64(x)        CLAMP((x),     0x0, U64_MAX)
#define CLAMP_S64(x)        CLAMP((x), S64_MIN, S64_MAX)
#define CLAMP_F32(x)        CLAMP((x), F32_MIN, F32_MAX)
#define CLAMP_F64(x)        CLAMP((x), F64_MIN, F64_MAX)

#define MTXF_END(mtx) {                         \
    (mtx)[0][3] = (mtx)[1][3] = (mtx)[2][3] = 0;\
    ((u32 *)(mtx))[15] = FLOAT_ONE;             \
}

/// From Wiseguy
ALWAYS_INLINE s32 roundf(f32 in) {
    f32 tmp;
    s32 out;
    __asm__("round.w.s %0,%1" : "=f" (tmp) : "f" (in ));
    __asm__("mfc1      %0,%1" : "=r" (out) : "f" (tmp));
    return out;
}
// backwards compatibility
#define round_float(in) roundf(in)

#define NEAR_ZERO   __FLT_EPSILON__
#define DEGREES(x) ((x) * 0x10000 / 360)
#define FLOAT_ONE   0x3F800000

#define sqr(x)              (    (x) * (x))
#define cube(x)             ( sqr(x) * (x))
#define quad(x)             (cube(x) * (x))

#define average_2(a, b      )   (((a) + (b)            ) / 2.0f)
#define average_3(a, b, c   )   (((a) + (b) + (c)      ) / 3.0f)
#define average_4(a, b, c, d)   (((a) + (b) + (c) + (d)) / 4.0f)

#define vec2_same(v, s)     (((v)[0]) = ((v)[1])                       = (s))
#define vec3_same(v, s)     (((v)[0]) = ((v)[1]) = ((v)[2])            = (s))
#define vec4_same(v, s)     (((v)[0]) = ((v)[1]) = ((v)[2]) = ((v)[3]) = (s))

#define vec2_zero(v)        (vec2_same((v), 0))
#define vec3_zero(v)        (vec3_same((v), 0))
#define vec4_zero(v)        (vec4_same((v), 0))

#define vec2_c(v)           (   (v)[0] + (v)[1])
#define vec3_c(v)           (vec2_c(v) + (v)[2])
#define vec4_c(v)           (vec3_c(v) + (v)[3])

#define vec2_average(v)     (vec2_c(v) / 2.0f)
#define vec3_average(v)     (vec3_c(v) / 3.0f)
#define vec4_average(v)     (vec4_c(v) / 4.0f)

#define vec2_sumsq(v)       (  sqr((v)[0]) + sqr((v)[1]))
#define vec3_sumsq(v)       (vec2_sumsq(v) + sqr((v)[2]))
#define vec4_sumsq(v)       (vec3_sumsq(v) + sqr((v)[3]))

#define vec2_mag(v)         (sqrtf(vec2_sumsq(v)))
#define vec3_mag(v)         (sqrtf(vec3_sumsq(v)))
#define vec4_mag(v)         (sqrtf(vec4_sumsq(v)))

#define absx(x) ((x) < 0 ? -(x) : (x))

#include "../../include/libc/stdlib.h"

#define vec2_set(dst, x, y) {           \
    (dst)[0] = (x);                     \
    (dst)[1] = (y);                     \
}
#define vec3_set(dst, x, y, z) {        \
    vec2_set((dst), (x), (y));          \
    (dst)[2] = (z);                     \
}
#define vec4_set(dst, x, y, z, w) {     \
    vec3_set((dst), (x), (y), (z));     \
    (dst)[3] = (w);                     \
}

#define vec2_copy(dst, src) {           \
    (dst)[0] = (src)[0];                \
    (dst)[1] = (src)[1];                \
}
#define vec3_copy(dst, src) {           \
    vec2_copy((dst), (src));            \
    (dst)[2] = (src)[2];                \
}
#define vec4_copy(dst, src) {           \
    vec3_copy((dst), (src));            \
    (dst)[3] = (src)[3];                \
}

#define vec3_copy_y_off(dst, src, y) {  \
    (dst)[0] =  (src)[0];               \
    (dst)[1] = ((src)[1] + (y));        \
    (dst)[2] =  (src)[2];               \
}

#define vec2_copy_roundf(dst, src) {    \
    (dst)[0] = roundf((src)[0]);        \
    (dst)[1] = roundf((src)[1]);        \
}
#define vec3_copy_roundf(dst, src) {    \
    vec2_copy_roundf((dst), (src));     \
    (dst)[2] = roundf((src)[2]);        \
}
#define vec4_copy_roundf(dst, src) {    \
    vec3_copy_roundf((dst), (src));     \
    (dst)[3] = roundf((src)[3]);        \
}

#define vec2_copy_inverse(dst, src) {   \
    (dst)[0] = (src)[1];                \
    (dst)[1] = (src)[0];                \
}
#define vec3_copy_inverse(dst, src) {   \
    (dst)[0] = (src)[2];                \
    (dst)[1] = (src)[1];                \
    (dst)[2] = (src)[0];                \
}
#define vec4_copy_inverse(dst, src) {   \
    (dst)[0] = (src)[3];                \
    (dst)[1] = (src)[2];                \
    (dst)[2] = (src)[1];                \
    (dst)[3] = (src)[0];                \
}

#define vec3_copy_offset_m1(dst, src) { \
    (dst)[0] = (src)[1];                \
    (dst)[1] = (src)[2];                \
    (dst)[2] = (src)[0];                \
}

#define vec2_copy_negative(dst, src) {  \
    (dst)[0] = -(src)[0];               \
    (dst)[1] = -(src)[1];               \
}
#define vec3_copy_negative(dst, src) {  \
    vec2_copy_negative((dst), (src));   \
    (dst)[2] = -(src)[2];               \
}
#define vec4_copy_negative(dst, src) {  \
    vec3_copy_negative((dst), (src));   \
    (dst)[3] = -(src)[3];               \
}

#define vec2_sum(dst, src1, src2) {     \
    (dst)[0] = ((src1)[0] + (src2)[0]); \
    (dst)[1] = ((src1)[1] + (src2)[1]); \
}
#define vec3_sum(dst, src1, src2) {     \
    vec2_sum((dst), (src1), (src2));    \
    (dst)[2] = ((src1)[2] + (src2)[2]); \
}
#define vec4_sum(dst, src1, src2) {     \
    vec3_sum((dst), (src1), (src2));    \
    (dst)[3] = ((src1)[3] + (src2)[3]); \
}
#define vec2_diff(dst, src1, src2) {    \
    (dst)[0] = ((src1)[0] - (src2)[0]); \
    (dst)[1] = ((src1)[1] - (src2)[1]); \
}
#define vec3_diff(dst, src1, src2) {    \
    vec2_diff((dst), (src1), (src2));   \
    (dst)[2] = ((src1)[2] - (src2)[2]); \
}
#define vec4_diff(dst, src1, src2) {    \
    vec3_diff((dst), (src1), (src2));   \
    (dst)[3] = ((src1)[3] - (src2)[3]); \
}
#define vec2_add(dst, src) vec2_sum((dst), (dst), (src))
#define vec3_add(dst, src) vec3_sum((dst), (dst), (src))
#define vec4_add(dst, src) vec4_sum((dst), (dst), (src))
#define vec2_prod_val(dst, src, x) {    \
    (dst)[0] = ((src)[0] * (x));        \
    (dst)[1] = ((src)[1] * (x));        \
}
#define vec3_prod_val(dst, src, x) {    \
    vec2_prod_val((dst), (src), (x));   \
    (dst)[2] = ((src)[2] * (x));        \
}
#define vec4_prod_val(dst, src, x) {    \
    vec3_prod_val((dst), (src), (x));   \
    (dst)[3] = ((src)[3] * (x));        \
}
void vec3f_diff(Vec3f dest, const Vec3f a, const Vec3f b);
void vec3i_diff(Vec3i dest, const Vec3i a, const Vec3i b);
void vec3s_diff(Vec3s dest, const Vec3s a, const Vec3s b);
void vec3f_copy(Vec3f dest, const Vec3f src);
void vec3f_set(Vec3f dest, const f32 x, const f32 y, const f32 z);
void vec3f_add(Vec3f dest, const Vec3f a);
void vec3f_sum(Vec3f dest, const Vec3f a, const Vec3f b);
void vec3f_scale(Vec3f dest, const f32 a);
void vec3s_copy(Vec3s dest, const Vec3s src);
void vec3s_set(Vec3s dest, const s16 x, const s16 y, const s16 z);
void vec3s_add(Vec3s dest, const Vec3s a);
void vec3s_sum(Vec3s dest, const Vec3s a, const Vec3s b);
void vec3s_sub(Vec3s dest, const Vec3s a);
void vec3s_to_vec3f(Vec3f dest, const Vec3s a);
void vec3f_to_vec3s(Vec3s dest, const Vec3f a);
void find_vector_perpendicular_to_plane(Vec3f dest, Vec3f a, Vec3f b, Vec3f c);
void vec3f_cross(Vec3f dest, Vec3f a, Vec3f b);
void vec3f_normalize(Vec3f dest);
f32 vec3f_length(Vec3f a);
f32 vec3f_dot(Vec3f a, Vec3f b);
void mtxf_copy(register Mat4 dest,register Mat4 src);
void mtxf_identity(register Mat4 mtx);
void mtxf_translate(Mat4 dest, Vec3f b);
void mtxf_lookat(Mat4 mtx, Vec3f from, Vec3f to, s16 roll);
void mtxf_rotate_zxy_and_translate(Mat4 dest, Vec3f translate, Vec3s rotate);
void mtxf_rotate_xyz_and_translate(Mat4 dest, Vec3f b, Vec3s c);
void mtxf_billboard(Mat4 dest, Mat4 mtx, Vec3f position, Vec3f scale, s16 angle);
void mtxf_cylboard(Mat4 dest, Mat4 mtx, Vec3f position, Vec3f scale, s16 angle);
void mtxf_align_terrain_normal(Mat4 dest, Vec3f upDir, Vec3f pos, s16 yaw);
void mtxf_align_terrain_triangle(Mat4 mtx, Vec3f pos, s16 yaw, f32 radius);
void mtxf_mul(Mat4 dest, Mat4 a, Mat4 b);
void mtxf_scale_vec3f(Mat4 dest, Mat4 mtx, Vec3f s);
void mtxf_mul_vec3s(Mat4 mtx, Vec3s b);
void mtxf_to_mtx(s16* dst, float* src);
void mtxf_rotate_xy(Mtx *mtx, s32 angle);
void get_pos_from_transform_mtx(Vec3f dest, Mat4 objMtx, Mat4 camMtx);
void vec3f_get_dist_and_angle(Vec3f from, Vec3f to, f32 *dist, s16 *pitch, s16 *yaw);
void vec3f_set_dist_and_angle(Vec3f from, Vec3f to, f32  dist, s16  pitch, s16  yaw);
s32 approach_s32(s32 current, s32 target, s32 inc, s32 dec);
f32 approach_f32(f32 current, f32 target, f32 inc, f32 dec);
s16 atan2s(f32 y, f32 x);
f32 atan2f(f32 a, f32 b);
void spline_get_weights(Vec4f result, f32 t, UNUSED s32 c);
void anim_spline_init(Vec4s *keyFrames);
s32 anim_spline_poll(Vec3f result);
void mtxf_rotate_zxy_and_translate_and_mul(Vec3s rot, Vec3f trans, Mat4 dest, Mat4 src);
void mtxf_rotate_xyz_and_translate_and_mul(Vec3s rot, Vec3f trans, Mat4 dest, Mat4 src);
#endif // MATH_UTIL_H
