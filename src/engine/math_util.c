#include <ultra64.h>

#include "sm64.h"
#include "engine/graph_node.h"
#include "math_util.h"
#include "surface_collision.h"

#include "trig_tables.inc.c"

// Variables for a spline curve animation (used for the flight path in the grand star cutscene)
Vec4s *gSplineKeyframe;
float gSplineKeyframeFraction;
int gSplineState;

// These functions have bogus return values.
// Disable the compiler warning.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-local-addr"


/// Copy vector 'src' to 'dest'
#define vec3_copy_bits(destFmt, dest, srcFmt, src) { \
    register destFmt x = ((srcFmt *) src)[0];        \
    register destFmt y = ((srcFmt *) src)[1];        \
    register destFmt z = ((srcFmt *) src)[2];        \
    ((destFmt *) dest)[0] = x;                       \
    ((destFmt *) dest)[1] = y;                       \
    ((destFmt *) dest)[2] = z;                       \
}
void vec3f_copy    (Vec3f dest, const Vec3f src) { vec3_copy_bits(f32, dest, f32, src); } // 32 -> 32
void vec3i_copy    (Vec3i dest, const Vec3i src) { vec3_copy_bits(s32, dest, s32, src); } // 32 -> 32
void vec3s_copy    (Vec3s dest, const Vec3s src) { vec3_copy_bits(s16, dest, s16, src); } // 16 -> 16
void vec3s_to_vec3i(Vec3i dest, const Vec3s src) { vec3_copy_bits(s32, dest, s16, src); } // 16 -> 32
void vec3s_to_vec3f(Vec3f dest, const Vec3s src) { vec3_copy_bits(f32, dest, s16, src); } // 16 -> 32
void vec3i_to_vec3s(Vec3s dest, const Vec3i src) { vec3_copy_bits(s16, dest, s32, src); } // 32 -> 16
void vec3i_to_vec3f(Vec3f dest, const Vec3i src) { vec3_copy_bits(f32, dest, s32, src); } // 32 -> 32

/// Convert float vector a to a short vector 'dest' by rounding the components to the nearest integer.
#define vec3_copy_bits_roundf(fmt, dest, src) { \
    register fmt x = roundf(src[0]);            \
    register fmt y = roundf(src[1]);            \
    register fmt z = roundf(src[2]);            \
    ((fmt *) dest)[0] = x;                      \
    ((fmt *) dest)[1] = y;                      \
    ((fmt *) dest)[2] = z;                      \
}
void vec3f_to_vec3s(Vec3s dest, const Vec3f src) { vec3_copy_bits_roundf(s16, dest, src); } // 32 -> 16
void vec3f_to_vec3i(Vec3i dest, const Vec3f src) { vec3_copy_bits_roundf(s32, dest, src); } // 32 -> 32
#undef vec3_copy_bits_roundf

#define vec3_set(dst, x, y, z) {        \
    vec2_set((dst), (x), (y));          \
    (dst)[2] = (z);                     \
}

/// Set vector 'dest' to (x, y, z)
inline void vec3f_set(Vec3f dest, const f32 x, const f32 y, const f32 z) { vec3_set(dest, x, y, z); }
inline void vec3i_set(Vec3i dest, const s32 x, const s32 y, const s32 z) { vec3_set(dest, x, y, z); }
inline void vec3s_set(Vec3s dest, const s16 x, const s16 y, const s16 z) { vec3_set(dest, x, y, z); }

/// Add vector 'a' to 'dest'
#define vec3_add_func(fmt, dest, a) {   \
    register fmt *temp = (fmt *)(dest); \
    register fmt sum, sum2;             \
    register s32 i;                     \
    for (i = 0; i < 3; i++) {           \
        sum = *(a);                     \
        (a)++;                          \
        sum2 = *temp;                   \
        *temp = (sum + sum2);           \
        temp++;                         \
    }                                   \
}
void vec3f_add(Vec3f dest, const Vec3f a) { vec3_add_func(f32, dest, a); }
void vec3i_add(Vec3i dest, const Vec3i a) { vec3_add_func(s32, dest, a); }
void vec3s_add(Vec3s dest, const Vec3s a) { vec3_add_func(s16, dest, a); }
#undef vec3_add_func

/// Make 'dest' the sum of vectors a and b.
#define vec3_sum_func(fmt, dest, a, b) {\
    register fmt *temp = (fmt *)(dest); \
    register fmt sum, sum2;             \
    register s32 i;                     \
    for (i = 0; i < 3; i++) {           \
        sum = *(a);                     \
        (a)++;                          \
        sum2 = *(b);                    \
        (b)++;                          \
        *temp = (sum + sum2);           \
        temp++;                         \
    }                                   \
}
void vec3f_sum(Vec3f dest, const Vec3f a, const Vec3f b) { vec3_sum_func(f32, dest, a, b); }
void vec3i_sum(Vec3i dest, const Vec3i a, const Vec3i b) { vec3_sum_func(s32, dest, a, b); }
void vec3s_sum(Vec3s dest, const Vec3s a, const Vec3s b) { vec3_sum_func(s16, dest, a, b); }
#undef vec3_sum_func

/// Subtract vector a from 'dest'
#define vec3_sub_func(fmt, dest, a) {   \
    register fmt x = ((fmt *) a)[0];    \
    register fmt y = ((fmt *) a)[1];    \
    register fmt z = ((fmt *) a)[2];    \
    ((fmt *) dest)[0] -= x;             \
    ((fmt *) dest)[1] -= y;             \
    ((fmt *) dest)[2] -= z;             \
}
void vec3f_sub(Vec3f dest, const Vec3f a) { vec3_sub_func(f32, dest, a); }
void vec3i_sub(Vec3i dest, const Vec3i a) { vec3_sub_func(s32, dest, a); }
void vec3s_sub(Vec3s dest, const Vec3s a) { vec3_sub_func(s16, dest, a); }
#undef vec3_sub_func

/// Make 'dest' the difference of vectors a and b.
#define vec3_diff_func(fmt, dest, a, b) {   \
    register fmt x1 = ((fmt *) a)[0];       \
    register fmt y1 = ((fmt *) a)[1];       \
    register fmt z1 = ((fmt *) a)[2];       \
    register fmt x2 = ((fmt *) b)[0];       \
    register fmt y2 = ((fmt *) b)[1];       \
    register fmt z2 = ((fmt *) b)[2];       \
    ((fmt *) dest)[0] = (x1 - x2);          \
    ((fmt *) dest)[1] = (y1 - y2);          \
    ((fmt *) dest)[2] = (z1 - z2);          \
}
void vec3f_diff(Vec3f dest, const Vec3f a, const Vec3f b) { vec3_diff_func(f32, dest, a, b); }
void vec3i_diff(Vec3i dest, const Vec3i a, const Vec3i b) { vec3_diff_func(s32, dest, a, b); }
void vec3s_diff(Vec3s dest, const Vec3s a, const Vec3s b) { vec3_diff_func(s16, dest, a, b); }
#undef vec3_diff_func

/// Multiply vector 'a' into 'dest'
#define vec3_mul_func(fmt, dest, a) {   \
    register fmt x = ((fmt *) a)[0];    \
    register fmt y = ((fmt *) a)[1];    \
    register fmt z = ((fmt *) a)[2];    \
    ((fmt *) dest)[0] *= x;             \
    ((fmt *) dest)[1] *= y;             \
    ((fmt *) dest)[2] *= z;             \
}
void vec3f_mul(Vec3f dest, const Vec3f a) { vec3_mul_func(f32, dest, a); }
void vec3i_mul(Vec3i dest, const Vec3i a) { vec3_mul_func(s32, dest, a); }
void vec3s_mul(Vec3s dest, const Vec3s a) { vec3_mul_func(s16, dest, a); }
#undef vec3_mul_func

/// Make 'dest' the product of vectors a and b.
#define vec3_prod_func(fmt, dest, a, b) {   \
    register fmt x1 = ((fmt *) a)[0];       \
    register fmt y1 = ((fmt *) a)[1];       \
    register fmt z1 = ((fmt *) a)[2];       \
    register fmt x2 = ((fmt *) b)[0];       \
    register fmt y2 = ((fmt *) b)[1];       \
    register fmt z2 = ((fmt *) b)[2];       \
    ((fmt *) dest)[0] = (x1 * x2);          \
    ((fmt *) dest)[1] = (y1 * y2);          \
    ((fmt *) dest)[2] = (z1 * z2);          \
}
void vec3f_prod(Vec3f dest, const Vec3f a, const Vec3f b) { vec3_prod_func(f32, dest, a, b); }
void vec3i_prod(Vec3i dest, const Vec3i a, const Vec3i b) { vec3_prod_func(s32, dest, a, b); }
void vec3s_prod(Vec3s dest, const Vec3s a, const Vec3s b) { vec3_prod_func(s16, dest, a, b); }
#undef vec3_prod_func


// / Add vector 'a' to 'dest'
// #define vec3_div_func(fmt, dest, a) {   \
    // register fmt x = ((fmt *) a)[0];    \
    // register fmt y = ((fmt *) a)[1];    \
    // register fmt z = ((fmt *) a)[2];    \
    // ((fmt *) dest)[0] /= x;             \
    // ((fmt *) dest)[1] /= y;             \
    // ((fmt *) dest)[2] /= z;             \
// }
// void vec3f_div(Vec3f dest, const Vec3f a) { vec3_div_func(f32, dest, a); }
// void vec3i_div(Vec3i dest, const Vec3i a) { vec3_div_func(s32, dest, a); }
// void vec3s_div(Vec3s dest, const Vec3s a) { vec3_div_func(s16, dest, a); }
// #undef vec3_div_func

// / Make 'dest' the sum of vectors a and b.
// #define vec3_quot_func(fmt, dest, a, b) {   \
    // register fmt x1 = ((fmt *) a)[0];       \
    // register fmt y1 = ((fmt *) a)[1];       \
    // register fmt z1 = ((fmt *) a)[2];       \
    // register fmt x2 = ((fmt *) b)[0];       \
    // register fmt y2 = ((fmt *) b)[1];       \
    // register fmt z2 = ((fmt *) b)[2];       \
    // ((fmt *) dest)[0] = (x1 / x2);          \
    // ((fmt *) dest)[1] = (y1 / y2);          \
    // ((fmt *) dest)[2] = (z1 / z2);          \
// }
// void vec3f_quot(Vec3f dest, const Vec3f a, const Vec3f b) { vec3_quot_func(f32, dest, a, b); }
// void vec3i_quot(Vec3i dest, const Vec3i a, const Vec3i b) { vec3_quot_func(s32, dest, a, b); }
// void vec3s_quot(Vec3s dest, const Vec3s a, const Vec3s b) { vec3_quot_func(s16, dest, a, b); }
// #undef vec3_quot_func

/// remnant I don't feel like changing
void vec3f_scale(Vec3f dest, const f32 a)
{
    dest[0] *= a;
    dest[1] *= a;
    dest[2] *= a;
}


/**
 * Set 'dest' the normal vector of a triangle with vertices a, b and c.
 * It is similar to vec3f_cross, but it calculates the vectors (c-b) and (b-a)
 * at the same time.
 */
void find_vector_perpendicular_to_plane(Vec3f dest, Vec3f a, Vec3f b, Vec3f c) {
    dest[0] = (b[1] - a[1]) * (c[2] - b[2]) - (c[1] - b[1]) * (b[2] - a[2]);
    dest[1] = (b[2] - a[2]) * (c[0] - b[0]) - (c[2] - b[2]) * (b[0] - a[0]);
    dest[2] = (b[0] - a[0]) * (c[1] - b[1]) - (c[0] - b[0]) * (b[1] - a[1]);
}

/// Make vector 'dest' the cross product of vectors a and b.
void vec3f_cross(Vec3f dest, Vec3f a, Vec3f b) {
    dest[0] = a[1] * b[2] - b[1] * a[2];
    dest[1] = a[2] * b[0] - b[2] * a[0];
    dest[2] = a[0] * b[1] - b[0] * a[1];
}

/// Scale vector 'dest' so it has length 1
void vec3f_normalize(Vec3f dest) {
    //! Possible division by zero
    f32 invsqrt = 1.0f / sqrtf(dest[0] * dest[0] + dest[1] * dest[1] + dest[2] * dest[2]);

    dest[0] *= invsqrt;
    dest[1] *= invsqrt;
    dest[2] *= invsqrt;
}

/// Get length of vector 'a'
f32 vec3f_length(Vec3f a)
{
	return sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

/// Get dot product of vectors 'a' and 'b'
f32 vec3f_dot(Vec3f a, Vec3f b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

#pragma GCC diagnostic pop

/// Struct the same data size as a Mat4
struct CopyMat4 {
    f32 a[0x10];
};

/// Copy matrix 'src' to 'dest' by casting to a struct CopyMat4 pointer.
void mtxf_copy(register Mat4 dest, register Mat4 src) {
    *((struct CopyMat4 *) dest) = *((struct CopyMat4 *) src);
}

/**
 * Set mtx to the identity matrix
 */
/// Set mtx to the identity matrix.
void mtxf_identity(register Mat4 mtx) {
    s32 i;
    f32 *dest;
    for (dest = ((f32 *) mtx + 1), i = 0; i < 14; dest++, i++) {
        *dest = 0;
    }
    for (dest = (f32 *) mtx, i = 0; i < 4; dest += 5, i++) {
        *((u32 *) dest) = FLOAT_ONE;
    }
}

/**
 * Set dest to a translation matrix of vector b
 */
/// Set dest to a translation matrix of vector b.
void mtxf_translate(Mat4 dest, Vec3f b) {
    register s32 i;
    register f32 *pen;
    for (pen = ((f32 *) dest + 1), i = 0; i < 12; pen++, i++) {
        *pen = 0;
    }
    for (pen = (f32 *) dest, i = 0; i < 4; pen += 5, i++) {
        *((u32 *) pen) = FLOAT_ONE;
    }
    vec3f_copy(&dest[3][0], &b[0]);
}

/**
 * Set mtx to a look-at matrix for the camera. The resulting transformation
 * transforms the world as if there exists a camera at position 'from' pointed
 * at the position 'to'. The up-vector is assumed to be (0, 1, 0), but the 'roll'
 * angle allows a bank rotation of the camera.
 */
void mtxf_lookat(Mat4 mtx, Vec3f from, Vec3f to, s16 roll) {
    Vec3f colX, colY, colZ;
    register f32 dx = (to[0] - from[0]);
    register f32 dz = (to[2] - from[2]);
    register f32 invLength = sqrtf(sqr(dx) + sqr(dz));
    invLength = -(1.0f / MAX(invLength, NEAR_ZERO));
    dx *= invLength;
    dz *= invLength;
    f32 sr  = sins(roll);
    colY[1] = coss(roll);
    colY[0] = ( sr * dz);
    colY[2] = (-sr * dx);
    vec3f_diff(colZ, from, to); // to & from are swapped
    vec3f_normalize(colZ);
    vec3f_cross(colX, colY, colZ);
    vec3f_normalize(colX);
    vec3f_cross(colY, colZ, colX);
    vec3f_normalize(colY);
    mtx[0][0] = colX[0];
    mtx[1][0] = colX[1];
    mtx[2][0] = colX[2];
    mtx[0][1] = colY[0];
    mtx[1][1] = colY[1];
    mtx[2][1] = colY[2];
    mtx[0][2] = colZ[0];
    mtx[1][2] = colZ[1];
    mtx[2][2] = colZ[2];
    mtx[3][0] = -vec3f_dot(from, colX);
    mtx[3][1] = -vec3f_dot(from, colY);
    mtx[3][2] = -vec3f_dot(from, colZ);
    MTXF_END(mtx);
}

/// Build a matrix that rotates around the z axis, then the x axis, then the y axis, and then translates.
void mtxf_rotate_zxy_and_translate(Mat4 dest, Vec3f trans, Vec3s rot) {
    register f32 sx   = sins(rot[0]);
    register f32 cx   = coss(rot[0]);
    register f32 sy   = sins(rot[1]);
    register f32 cy   = coss(rot[1]);
    register f32 sz   = sins(rot[2]);
    register f32 cz   = coss(rot[2]);
    register f32 sysz = (sy * sz);
    register f32 cycz = (cy * cz);
    dest[0][0] = ((sysz * sx) + cycz);
    register f32 cysz = (cy * sz);
    register f32 sycz = (sy * cz);
    dest[1][0] = ((sycz * sx) - cysz);
    dest[2][0] = (cx * sy);
    dest[0][1] = (cx * sz);
    dest[1][1] = (cx * cz);
    dest[2][1] = -sx;
    dest[0][2] = ((cysz * sx) - sycz);
    dest[1][2] = ((cycz * sx) + sysz);
    dest[2][2] = (cx * cy);
    vec3f_copy(dest[3], trans);
    MTXF_END(dest);
}

/// Build a matrix that rotates around the x axis, then the y axis, then the z axis, and then translates.
UNUSED void mtxf_rotate_xyz_and_translate(Mat4 dest, Vec3f trans, Vec3s rot) {
    register f32 sx   = sins(rot[0]);
    register f32 cx   = coss(rot[0]);
    register f32 sy   = sins(rot[1]);
    register f32 cy   = coss(rot[1]);
    register f32 sz   = sins(rot[2]);
    register f32 cz   = coss(rot[2]);
    dest[0][0] = (cy * cz);
    dest[0][1] = (cy * sz);
    dest[0][2] = -sy;
    register f32 sxcz = (sx * cz);
    register f32 cxsz = (cx * sz);
    dest[1][0] = ((sxcz * sy) - cxsz);
    register f32 sxsz = (sx * sz);
    register f32 cxcz = (cx * cz);
    dest[1][1] = ((sxsz * sy) + cxcz);
    dest[1][2] = (sx * cy);
    dest[2][0] = ((cxcz * sy) + sxsz);
    dest[2][1] = ((cxsz * sy) - sxcz);
    dest[2][2] = (cx * cy);
    vec3f_copy(dest[3], trans);
    MTXF_END(dest);
}

/**
 * Set 'dest' to a transformation matrix that turns an object to face the camera.
 * 'mtx' is the look-at matrix from the camera.
 * 'position' is the position of the object in the world.
 * 'scale' is the scale of the object.
 * 'angle' rotates the object while still facing the camera.
 */
void mtxf_billboard(Mat4 dest, Mat4 mtx, Vec3f position, Vec3f scale, s16 angle) {
    register s32 i;
    register f32 sx = scale[0];
    register f32 sy = scale[1];
    register f32 sz = ((f32 *) scale)[2];
    register f32 *temp2, *temp = (f32 *)dest;
    for (i = 0; i < 16; i++) {
        *temp = 0;
        temp++;
    }
    if (angle == 0x0) {
        // ((u32 *) dest)[0] = FLOAT_ONE;
        dest[0][0] = sx; // [0][0]
        dest[0][1] = 0;
        dest[1][0] = 0;
        // ((u32 *) dest)[5] = FLOAT_ONE;
        dest[1][1] = sy; // [1][1]
    } else {
        dest[0][0] = (coss(angle) * sx);
        dest[0][1] = (sins(angle) * sx);
        dest[1][0] = (-dest[0][1] * sy);
        dest[1][1] = ( dest[0][0] * sy);
    }
    // ((u32 *) dest)[10] = FLOAT_ONE;
    // dest[2][2] = sz; // [2][2]
    ((f32 *) dest)[10] = sz; // [2][2]
    // ((f32 *) dest)[10] = FLOAT_ONE; // [2][2]
    dest[2][3] = 0;
    ((u32 *) dest)[15] = FLOAT_ONE; // [3][3]

    temp  = (f32 *)dest;
    temp2 = (f32 *)mtx;
    for (i = 0; i < 3; i++) {
        temp[12] = ((temp2[ 0] * position[0])
                  + (temp2[ 4] * position[1])
                  + (temp2[ 8] * position[2])
                  +  temp2[12]);
        temp++;
        temp2++;
    }
}

// straight up mtxf_billboard but minus the dest[1][n] lines. transform for cylindrical billboards
void mtxf_cylboard(Mat4 dest, Mat4 mtx, Vec3f position, s16 angle) {
    dest[0][0] = coss(angle);
    dest[0][1] = sins(angle);
    dest[0][2] = 0;
    dest[0][3] = 0;

    dest[1][0] = mtx[1][0];
    dest[1][1] = mtx[1][1];
    dest[1][2] = mtx[1][2];
    dest[1][3] = 0;

    dest[2][0] = 0;
    dest[2][1] = 0;
    dest[2][2] = 1;
    dest[2][3] = 0;

    dest[3][0] =
        mtx[0][0] * position[0] + mtx[1][0] * position[1] + mtx[2][0] * position[2] + mtx[3][0];
    dest[3][1] =
        mtx[0][1] * position[0] + mtx[1][1] * position[1] + mtx[2][1] * position[2] + mtx[3][1];
    dest[3][2] =
        mtx[0][2] * position[0] + mtx[1][2] * position[1] + mtx[2][2] * position[2] + mtx[3][2];
    dest[3][3] = 1;
}

/**
 * Set 'dest' to a transformation matrix that aligns an object with the terrain
 * based on the normal. Used for enemies.
 * 'upDir' is the terrain normal
 * 'yaw' is the angle which it should face
 * 'pos' is the object's position in the world
 */
void mtxf_align_terrain_normal(Mat4 dest, Vec3f upDir, Vec3f pos, s16 yaw) {
    Vec3f lateralDir;
    Vec3f leftDir;
    Vec3f forwardDir;
    vec3f_set(lateralDir, sins(yaw), 0.0f, coss(yaw));
    vec3f_normalize(upDir);
    vec3f_cross(leftDir, upDir, lateralDir);
    vec3f_normalize(leftDir);
    vec3f_cross(forwardDir, leftDir, upDir);
    vec3f_normalize(forwardDir);
    vec3f_copy(dest[0], leftDir);
    vec3f_copy(dest[1], upDir);
    vec3f_copy(dest[2], forwardDir);
    vec3f_copy(dest[3], pos);
    MTXF_END(dest);
}

/**
 * Set 'mtx' to a transformation matrix that aligns an object with the terrain
 * based on 3 height samples in an equilateral triangle around the object.
 * Used for Mario when crawling or sliding.
 * 'yaw' is the angle which it should face
 * 'pos' is the object's position in the world
 * 'radius' is the distance from each triangle vertex to the center
 */
void mtxf_align_terrain_triangle(Mat4 mtx, Vec3f pos, s16 yaw, f32 radius) {
    struct Surface *floor;
    Vec3f point0, point1, point2;
    Vec3f forward;
    Vec3f xColumn, yColumn, zColumn;
    f32 minY   = (-radius * 3);
    f32 height = (pos[1] + 150);

    point0[0] = (pos[0] + (radius * sins(yaw + DEGREES( 60))));
    point0[2] = (pos[2] + (radius * coss(yaw + DEGREES( 60))));
    point0[1] = find_floor(point0[0], height, point0[2], &floor);
    point1[0] = (pos[0] + (radius * sins(yaw + DEGREES(180))));
    point1[2] = (pos[2] + (radius * coss(yaw + DEGREES(180))));
    point1[1] = find_floor(point1[0], height, point1[2], &floor);
    point2[0] = (pos[0] + (radius * sins(yaw + DEGREES(-60))));
    point2[2] = (pos[2] + (radius * coss(yaw + DEGREES(-60))));
    point2[1] = find_floor(point2[0], height, point2[2], &floor);

    if ((point0[1] - pos[1]) < minY) point0[1] = pos[1];
    if ((point1[1] - pos[1]) < minY) point1[1] = pos[1];
    if ((point2[1] - pos[1]) < minY) point2[1] = pos[1];

    f32 avgY = average_3(point0[1], point1[1], point2[1]);

    vec3f_set(forward, sins(yaw), 0.0f, coss(yaw));
    find_vector_perpendicular_to_plane(yColumn, point0, point1, point2);
    vec3f_normalize(yColumn);
    vec3f_cross(xColumn, yColumn, forward);
    vec3f_normalize(xColumn);
    vec3f_cross(zColumn, xColumn, yColumn);
    vec3f_normalize(zColumn);
    vec3f_copy(mtx[0], xColumn);
    vec3f_copy(mtx[1], yColumn);
    vec3f_copy(mtx[2], zColumn);

    mtx[3][0] = pos[0];
    mtx[3][1] = MAX(pos[1], avgY);
    mtx[3][2] = pos[2];

    MTXF_END(mtx);
}
/**
 * Sets matrix 'dest' to the matrix product b * a assuming they are both
 * transformation matrices with a w-component of 1. Since the bottom row
 * is assumed to equal [0, 0, 0, 1], it saves some multiplications and
 * addition.
 * The resulting matrix represents first applying transformation b and
 * then a.
 */
void mtxf_mul(Mat4 dest, Mat4 a, Mat4 b) {
    Vec3f entry;
    register f32 *temp  = (f32 *)a;
    register f32 *temp2 = (f32 *)dest;
    register f32 *temp3;
    register s32 i;
    for (i = 0; i < 16; i++) {
        vec3_copy(entry, temp);
        for (temp3 = (f32 *)b; (i & 3) != 3; i++) {
            *temp2 = ((entry[0] * temp3[0])
                    + (entry[1] * temp3[4])
                    + (entry[2] * temp3[8]));
            temp2++;
            temp3++;
        }
        *temp2 = 0;
        temp += 4;
        temp2++;
    }
    vec3f_add(&dest[3][0], &b[3][0]);
    ((u32 *) dest)[15] = FLOAT_ONE;
}

/**
 * Set matrix 'dest' to 'mtx' scaled by vector s
 */
void mtxf_scale_vec3f(Mat4 dest, Mat4 mtx, register Vec3f s) {
    register f32 *temp  = (f32 *)dest;
    register f32 *temp2 = (f32 *)mtx;
    register s32 i;

    for (i = 0; i < 4; i++) {
        temp[ 0] = temp2[ 0] * s[0];
        temp[ 4] = temp2[ 4] * s[1];
        temp[ 8] = temp2[ 8] * s[2];
        temp[12] =  temp2[12];
        temp++;
        temp2++;
    }
}

/**
 * Multiply a vector with a transformation matrix, which applies the transformation
 * to the point. Note that the bottom row is assumed to be [0, 0, 0, 1], which is
 * true for transformation matrices if the translation has a w component of 1.
 */
UNUSED void mtxf_mul_vec3s(Mat4 mtx, Vec3s b) {
    register f32 x = b[0];
    register f32 y = b[1];
    register f32 z = b[2];
    register f32 *temp2 = (f32 *)mtx;
    register s32 i;
    register s16 *c = b;
    for (i = 0; i < 3; i++) {
        c[0] = ((x * temp2[ 0])
              + (y * temp2[ 4])
              + (z * temp2[ 8])
              +      temp2[12]);
        c++;
        temp2++;
    }
}

// Constructs a float in registers, which can be faster than gcc's default of loading a float from rodata.
// Especially fast for halfword floats, which get loaded with a `lui` + `mtc1`.
static ALWAYS_INLINE float construct_float(const float f)
{
    u32 r;
    float f_out;
    u32 i = *(u32*)(&f);

    if (!__builtin_constant_p(i))
    {
        return *(float*)(&i);
    }

    u32 upper = (i >> 16);
    u32 lower = (i >>  0) & 0xFFFF;

    if ((i & 0xFFFF) == 0) {
        __asm__ ("lui %0, %1"
                                : "=r"(r)
                                : "K"(upper));
    } else if ((i & 0xFFFF0000) == 0) {
        __asm__ ("addiu %0, $0, %1"
                                : "+r"(r)
                                : "K"(lower));
    } else {
        __asm__ ("lui %0, %1"
                                : "=r"(r)
                                : "K"(upper));
        __asm__ ("addiu %0, %0, %1"
                                : "+r"(r)
                                : "K"(lower));
    }

    __asm__ ("mtc1 %1, %0"
                         : "=f"(f_out)
                         : "r"(r));
    return f_out;
}

void mtxf_to_mtx(s16* dst, float* src)
{
    int i;
	float scale = construct_float(65536.0f / WORLD_SCALE);
    // Iterate over pairs of values in the input matrix
    for (i = 0; i < 8; i++)
    {
        // Read the first input in the current pair
        float a = src[2 * i + 0];

        // Convert the first input to fixed
        s32 a_int = (s32)(a * scale);
        dst[2 * i +  0] = (s16)(a_int >> 16);
        dst[2 * i + 16] = (s16)(a_int >>  0);

        // If this is the left half of the matrix, convert the second input to fixed
        if ((i & 1) == 0)
        {
            // Read the second input in the current pair
            float b = src[2 * i + 1];
            s32 b_int = (s32)(b * scale);
            dst[2 * i +  1] = (s16)(b_int >> 16);
            dst[2 * i + 17] = (s16)(b_int >>  0);
        }
        // Otherwise, skip the second input because column 4 will always be zero
        // Row 4 column 4 is handled after the loop.
        else
        {
            dst[2 * i +  1] = 0;
            dst[2 * i + 17] = 0;
        }

    }
    // Write 1.0 to the bottom right entry in the output matrix
    // The low half was already set to zero in the loop, so we only need
    //  to set the top half.
    dst[15] = 1;
}

/**
 * Set 'mtx' to a transformation matrix that rotates around the z axis.
 */
#define MATENTRY(a, b)                          \
    ((s16 *) mtx)[a     ] = (((s32) b) >> 16);  \
    ((s16 *) mtx)[a + 16] = (((s32) b) & 0xFFFF);
void mtxf_rotate_xy(Mtx *mtx, s32 angle) {
    register s32 i = (coss(angle) * 0x10000);
    register s32 j = (sins(angle) * 0x10000);
    register f32 *temp = (f32 *)mtx;
    register s32 k;
    for (k = 0; k < 16; k++) {
        *temp = 0;
        temp++;
    }
    MATENTRY(0,  i)
    MATENTRY(1,  j)
    MATENTRY(4, -j)
    MATENTRY(5,  i)
    ((s16 *) mtx)[10] = 1;
    ((s16 *) mtx)[15] = 1;
}

/**
 * Extract a position given an object's transformation matrix and a camera matrix.
 * This is used for determining the world position of the held object: since objMtx
 * inherits the transformation from both the camera and Mario, it calculates this
 * by taking the camera matrix and inverting its transformation by first rotating
 * objMtx back from screen orientation to world orientation, and then subtracting
 * the camera position.
 */
void get_pos_from_transform_mtx(Vec3f dest, Mat4 objMtx, Mat4 camMtx) {
    f32 camX = camMtx[3][0] * camMtx[0][0] + camMtx[3][1] * camMtx[0][1] + camMtx[3][2] * camMtx[0][2];
    f32 camY = camMtx[3][0] * camMtx[1][0] + camMtx[3][1] * camMtx[1][1] + camMtx[3][2] * camMtx[1][2];
    f32 camZ = camMtx[3][0] * camMtx[2][0] + camMtx[3][1] * camMtx[2][1] + camMtx[3][2] * camMtx[2][2];

    dest[0] =
        objMtx[3][0] * camMtx[0][0] + objMtx[3][1] * camMtx[0][1] + objMtx[3][2] * camMtx[0][2] - camX;
    dest[1] =
        objMtx[3][0] * camMtx[1][0] + objMtx[3][1] * camMtx[1][1] + objMtx[3][2] * camMtx[1][2] - camY;
    dest[2] =
        objMtx[3][0] * camMtx[2][0] + objMtx[3][1] * camMtx[2][1] + objMtx[3][2] * camMtx[2][2] - camZ;
}

/**
 * Take the vector starting at 'from' pointed at 'to' an retrieve the length
 * of that vector, as well as the yaw and pitch angles.
 * Basically it converts the direction to spherical coordinates.
 */

/// Finds the horizontal distance between two vectors.
void vec3f_get_lateral_dist(Vec3f from, Vec3f to, f32 *lateralDist) {
    register f32 dx = (to[0] - from[0]);
    register f32 dz = (to[2] - from[2]);
    *lateralDist = sqrtf(sqr(dx) + sqr(dz));
}

/// Finds the squared horizontal distance between two vectors. Avoids a sqrtf call.
void vec3f_get_lateral_dist_squared(Vec3f from, Vec3f to, f32 *lateralDist) {
    register f32 dx = (to[0] - from[0]);
    register f32 dz = (to[2] - from[2]);
    *lateralDist = (sqr(dx) + sqr(dz));
}

/// Finds the distance between two vectors.
void vec3f_get_dist(Vec3f from, Vec3f to, f32 *dist) {
    Vec3f d;
    vec3_diff(d, to, from);
    *dist = vec3_mag(d);
}

/// Finds the squared distance between two vectors. Avoids a sqrtf call.
void vec3f_get_dist_squared(Vec3f from, Vec3f to, f32 *dist) {
    Vec3f d;
    vec3_diff(d, to, from);
    *dist = vec3_sumsq(d);
}

/// Finds the distance and yaw etween two vectors.
void vec3f_get_dist_and_yaw(Vec3f from, Vec3f to, f32 *dist, s16 *yaw) {
    Vec3f d;
    vec3_diff(d, to, from);
    *dist = vec3_mag(d);
    *yaw = atan2s(d[2], d[0]);
}

/// Finds the pitch between two vectors.
void vec3f_get_pitch(Vec3f from, Vec3f to, s16 *pitch) {
    Vec3f d;
    vec3_diff(d, to, from);
    *pitch = atan2s(sqrtf(sqr(d[0]) + sqr(d[2])), d[1]);
}

/// Finds the yaw between two vectors.
void vec3f_get_yaw(Vec3f from, Vec3f to, s16 *yaw) {
    register f32 dx = (to[0] - from[0]);
    register f32 dz = (to[2] - from[2]);
    *yaw = atan2s(dz, dx);
}

/// Finds the pitch and yaw between two vectors.
void vec3f_get_angle(Vec3f from, Vec3f to, s16 *pitch, s16 *yaw) {
    Vec3f d;
    vec3_diff(d, to, from);
    *pitch = atan2s(sqrtf(sqr(d[0]) + sqr(d[2])), d[1]);
    *yaw   = atan2s(d[2], d[0]);
}

/// Finds the horizontal distance and pitch between two vectors.
void vec3f_get_lateral_dist_and_pitch(Vec3f from, Vec3f to, f32 *lateralDist, s16 *pitch) {
    Vec3f d;
    vec3_diff(d, to, from);
    *lateralDist = sqrtf(sqr(d[0]) + sqr(d[2]));
    *pitch       = atan2s(*lateralDist, d[1]);
}

/// Finds the horizontal distance and yaw between two vectors.
void vec3f_get_lateral_dist_and_yaw(Vec3f from, Vec3f to, f32 *lateralDist, s16 *yaw) {
    register f32 dx = (to[0] - from[0]);
    register f32 dz = (to[2] - from[2]);
    *lateralDist = sqrtf(sqr(dx) + sqr(dz));
    *yaw         = atan2s(dz, dx);
}

/// Finds the horizontal distance and angles between two vectors.
void vec3f_get_lateral_dist_and_angle(Vec3f from, Vec3f to, f32 *lateralDist, s16 *pitch, s16 *yaw) {
    Vec3f d;
    vec3_diff(d, to, from);
    *lateralDist = sqrtf(sqr(d[0]) + sqr(d[2]));
    *pitch       = atan2s(*lateralDist, d[1]);
    *yaw         = atan2s(d[2], d[0]);
}

/// Finds the distance and angles between two vectors.
void vec3f_get_dist_and_angle(Vec3f from, Vec3f to, f32 *dist, s16 *pitch, s16 *yaw) {
    Vec3f d;
    vec3_diff(d, to, from);
    register f32 xz = (sqr(d[0]) + sqr(d[2]));
    *dist           = sqrtf(xz + sqr(d[1]));
    *pitch          = atan2s(sqrtf(xz), d[1]);
    *yaw            = atan2s(d[2], d[0]);
}
void vec3s_get_dist_and_angle(Vec3s from, Vec3s to, s16 *dist, s16 *pitch, s16 *yaw) {
    Vec3s d;
    vec3_diff(d, to, from);
    register f32 xz = (sqr(d[0]) + sqr(d[2]));
    *dist           = sqrtf(xz + sqr(d[1]));
    *pitch          = atan2s(sqrtf(xz), d[1]);
    *yaw            = atan2s(d[2], d[0]);
}
void vec3f_to_vec3s_get_dist_and_angle(Vec3f from, Vec3s to, f32 *dist, s16 *pitch, s16 *yaw) {
    Vec3f d;
    vec3_diff(d, to, from);
    register f32 xz = (sqr(d[0]) + sqr(d[2]));
    *dist           = sqrtf(xz + sqr(d[1]));
    *pitch          = atan2s(sqrtf(xz), d[1]);
    *yaw            = atan2s(d[2], d[0]);
}

/// Finds the distance, horizontal distance, and angles between two vectors.
void vec3f_get_dist_and_lateral_dist_and_angle(Vec3f from, Vec3f to, f32 *dist, f32 *lateralDist, s16 *pitch, s16 *yaw) {
    Vec3f d;
    vec3_diff(d, to, from);
    register f32 xz = (sqr(d[0]) + sqr(d[2]));
    *dist           = sqrtf(xz + sqr(d[1]));
    *lateralDist    = sqrtf(xz);
    *pitch          = atan2s(*lateralDist, d[1]);
    *yaw            = atan2s(d[2], d[0]);
}

/**
 * Construct the 'to' point which is distance 'dist' away from the 'from' position,
 * and has the angles pitch and yaw.
 */
#define vec3_set_dist_and_angle(from, to, dist, pitch, yaw) { \
    register f32 dcos = (dist * coss(pitch)); \
    to[0] = (from[0] + (dcos * sins(yaw  ))); \
    to[1] = (from[1] + (dist * sins(pitch))); \
    to[2] = (from[2] + (dcos * coss(yaw  ))); \
}
void vec3f_set_dist_and_angle(Vec3f from, Vec3f to, f32 dist, s16 pitch, s16 yaw) {
    vec3_set_dist_and_angle(from, to, dist, pitch, yaw);
}

/**
 * Return the value 'current' after it tries to approach target, going up at
 * most 'inc' and going down at most 'dec'.
 */
s32 approach_s32(s32 current, s32 target, s32 inc, s32 dec) {
    //! If target is close to the max or min s32, then it's possible to overflow
    // past it without stopping.

    if (current < target) {
        current += inc;
        if (current > target) {
            current = target;
        }
    } else {
        current -= dec;
        if (current < target) {
            current = target;
        }
    }
    return current;
}

/**
 * Return the value 'current' after it tries to approach target, going up at
 * most 'inc' and going down at most 'dec'.
 */
f32 approach_f32(f32 current, f32 target, f32 inc, f32 dec) {
    if (current < target) {
        current += inc;
        if (current > target) {
            current = target;
        }
    } else {
        current -= dec;
        if (current < target) {
            current = target;
        }
    }
    return current;
}

/**
 * Helper function for atan2s. Does a look up of the arctangent of y/x assuming
 * the resulting angle is in range [0, 0x2000] (1/8 of a circle).
 */
static u16 atan2_lookup(f32 y, f32 x) {
    u16 ret;

    if (x == 0) {
        ret = gArctanTable[0];
    } else {
        ret = gArctanTable[(s32)(y / x * 1024 + 0.5f)];
    }
    return ret;
}

/**
 * Compute the angle from (0, 0) to (x, y) as a s16. Given that terrain is in
 * the xz-plane, this is commonly called with (z, x) to get a yaw angle.
 */
s16 atan2s(f32 y, f32 x) {
    u16 ret;

    if (x >= 0) {
        if (y >= 0) {
            if (y >= x) {
                ret = atan2_lookup(x, y);
            } else {
                ret = 0x4000 - atan2_lookup(y, x);
            }
        } else {
            y = -y;
            if (y < x) {
                ret = 0x4000 + atan2_lookup(y, x);
            } else {
                ret = 0x8000 - atan2_lookup(x, y);
            }
        }
    } else {
        x = -x;
        if (y < 0) {
            y = -y;
            if (y >= x) {
                ret = 0x8000 + atan2_lookup(x, y);
            } else {
                ret = 0xC000 - atan2_lookup(y, x);
            }
        } else {
            if (y < x) {
                ret = 0xC000 + atan2_lookup(y, x);
            } else {
                ret = -atan2_lookup(x, y);
            }
        }
    }
    return ret;
}

/**
 * Compute the atan2 in radians by calling atan2s and converting the result.
 */
f32 atan2f(f32 y, f32 x) {
    return (f32) atan2s(y, x) * M_PI / 0x8000;
}

#define CURVE_BEGIN_1 1
#define CURVE_BEGIN_2 2
#define CURVE_MIDDLE 3
#define CURVE_END_1 4
#define CURVE_END_2 5

/**
 * Set 'result' to a 4-vector with weights corresponding to interpolation
 * value t in [0, 1] and gSplineState. Given the current control point P, these
 * weights are for P[0], P[1], P[2] and P[3] to obtain an interpolated point.
 * The weights naturally sum to 1, and they are also always in range [0, 1] so
 * the interpolated point will never overshoot. The curve is guaranteed to go
 * through the first and last point, but not through intermediate points.
 *
 * gSplineState ensures that the curve is clamped: the first two points
 * and last two points have different weight formulas. These are the weights
 * just before gSplineState transitions:
 * 1: [1, 0, 0, 0]
 * 1->2: [0, 3/12, 7/12, 2/12]
 * 2->3: [0, 1/6, 4/6, 1/6]
 * 3->3: [0, 1/6, 4/6, 1/6] (repeats)
 * 3->4: [0, 1/6, 4/6, 1/6]
 * 4->5: [0, 2/12, 7/12, 3/12]
 * 5: [0, 0, 0, 1]
 *
 * I suspect that the weight formulas will give a 3rd degree B-spline with the
 * common uniform clamped knot vector, e.g. for n points:
 * [0, 0, 0, 0, 1, 2, ... n-1, n, n, n, n]
 * TODO: verify the classification of the spline / figure out how polynomials were computed
 */
void spline_get_weights(Vec4f result, f32 t, UNUSED s32 c) {
    f32 tinv = 1 - t;
    f32 tinv2 = tinv * tinv;
    f32 tinv3 = tinv2 * tinv;
    f32 t2 = t * t;
    f32 t3 = t2 * t;

    switch (gSplineState) {
        case CURVE_BEGIN_1:
            result[0] = tinv3;
            result[1] = t3 * 1.75f - t2 * 4.5f + t * 3.0f;
            result[2] = -t3 * (11 / 12.0f) + t2 * 1.5f;
            result[3] = t3 * (1 / 6.0f);
            break;
        case CURVE_BEGIN_2:
            result[0] = tinv3 * 0.25f;
            result[1] = t3 * (7 / 12.0f) - t2 * 1.25f + t * 0.25f + (7 / 12.0f);
            result[2] = -t3 * 0.5f + t2 * 0.5f + t * 0.5f + (1 / 6.0f);
            result[3] = t3 * (1 / 6.0f);
            break;
        case CURVE_MIDDLE:
            result[0] = tinv3 * (1 / 6.0f);
            result[1] = t3 * 0.5f - t2 + (4 / 6.0f);
            result[2] = -t3 * 0.5f + t2 * 0.5f + t * 0.5f + (1 / 6.0f);
            result[3] = t3 * (1 / 6.0f);
            break;
        case CURVE_END_1:
            result[0] = tinv3 * (1 / 6.0f);
            result[1] = -tinv3 * 0.5f + tinv2 * 0.5f + tinv * 0.5f + (1 / 6.0f);
            result[2] = tinv3 * (7 / 12.0f) - tinv2 * 1.25f + tinv * 0.25f + (7 / 12.0f);
            result[3] = t3 * 0.25f;
            break;
        case CURVE_END_2:
            result[0] = tinv3 * (1 / 6.0f);
            result[1] = -tinv3 * (11 / 12.0f) + tinv2 * 1.5f;
            result[2] = tinv3 * 1.75f - tinv2 * 4.5f + tinv * 3.0f;
            result[3] = t3;
            break;
    }
}

/**
 * Initialize a spline animation.
 * 'keyFrames' should be an array of (s, x, y, z) vectors
 *  s: the speed of the keyframe in 1000/frames, e.g. s=100 means the keyframe lasts 10 frames
 *  (x, y, z): point in 3D space on the curve
 * The array should end with three entries with s=0 (infinite keyframe duration).
 * That's because the spline has a 3rd degree polynomial, so it looks 3 points ahead.
 */
void anim_spline_init(Vec4s *keyFrames) {
    gSplineKeyframe = keyFrames;
    gSplineKeyframeFraction = 0;
    gSplineState = 1;
}

/**
 * Poll the next point from a spline animation.
 * anim_spline_init should be called before polling for vectors.
 * Returns TRUE when the last point is reached, FALSE otherwise.
 */
s32 anim_spline_poll(Vec3f result) {
    Vec4f weights;
    s32 i;
    s32 hasEnded = FALSE;

    vec3f_copy(result, gVec3fZero);
    spline_get_weights(weights, gSplineKeyframeFraction, gSplineState);
    for (i = 0; i < 4; i++) {
        result[0] += weights[i] * gSplineKeyframe[i][1];
        result[1] += weights[i] * gSplineKeyframe[i][2];
        result[2] += weights[i] * gSplineKeyframe[i][3];
    }

    if ((gSplineKeyframeFraction += gSplineKeyframe[0][0] / 1000.0f) >= 1) {
        gSplineKeyframe++;
        gSplineKeyframeFraction--;
        switch (gSplineState) {
            case CURVE_END_2:
                hasEnded = TRUE;
                break;
            case CURVE_MIDDLE:
                if (gSplineKeyframe[2][0] == 0) {
                    gSplineState = CURVE_END_1;
                }
                break;
            default:
                gSplineState++;
                break;
        }
    }

    return hasEnded;
}
