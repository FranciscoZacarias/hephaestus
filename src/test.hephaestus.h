#ifndef MATH_HEPHAESTUS_H
#define UI_HEPHAESTUS_H

/*
@Hephaestus generated code
*/

// Vector2
// -------typedef union Vec2u8  { u8  data[2]; struct { u8  x, y; }; } Vec2u8 ;
function Vec2u8  vector2_add(Vec2u8  a, Vec2u8  b);
function Vec2u8  vector2_subtract(Vec2u8  a, Vec2u8  b);
function Vec2u8  vector2_multiply(Vec2u8  a, Vec2u8  b);
function Vec2u8  vector2_divide(Vec2u8  a, Vec2u8  b);
function Vec2u8  vector2_scale(Vec2u8  vector, f32 scalar);
function Vec2u8  vector2_negate(Vec2u8  vector);
function Vec2u8  vector2_rotate(Vec2u8  vector, f32 radians);
function Vec2u8  vector2_normalize(Vec2u8  vector);
function Vec2u8  vector2_linear_interpolate(Vec2u8  a, Vec2u8  b, f32 t);
function Vec2u8  vector2_minimum(Vec2u8  a, Vec2u8  b);
function Vec2u8  vector2_maximum(Vec2u8  a, Vec2u8  b);
function Vec2u8  vector2_clamp(Vec2u8  vector, Vec2u8  minimum, Vec2u8  maximum);
function Vec2u8  vector2_absolute(Vec2u8  vector);
function Vec2u8  vector2_perpendicular(Vec2u8  vector);
function Vec2u8  vector2_reflect(Vec2u8  incident, Vec2u8  normal);
function Vec2u8  vector2_project(Vec2u8  vector, Vec2u8  onto);
function Vec2u8  vector2_snap_to_grid(Vec2u8  vector, f32 grid_size);
function f32 vector2_dot_product(Vec2u8  a, Vec2u8  b);
function f32 vector2_cross_product(Vec2u8  a, Vec2u8  b);
function f32 vector2_length(Vec2u8  vector);
function f32 vector2_length_squared(Vec2u8  vector);
function f32 vector2_distance(Vec2u8  a, Vec2u8  b);
function f32 vector2_distance_squared(Vec2u8  a, Vec2u8  b);
function f32 vector2_angle_between(Vec2u8  a, Vec2u8  b);
function f32 vector2_signed_angle_between(Vec2u8  a, Vec2u8  b);

typedef union Vec2u16 { u16 data[2]; struct { u16 x, y; }; } Vec2u16;
function Vec2u16 vector2_add(Vec2u16 a, Vec2u16 b);
function Vec2u16 vector2_subtract(Vec2u16 a, Vec2u16 b);
function Vec2u16 vector2_multiply(Vec2u16 a, Vec2u16 b);
function Vec2u16 vector2_divide(Vec2u16 a, Vec2u16 b);
function Vec2u16 vector2_scale(Vec2u16 vector, f32 scalar);
function Vec2u16 vector2_negate(Vec2u16 vector);
function Vec2u16 vector2_rotate(Vec2u16 vector, f32 radians);
function Vec2u16 vector2_normalize(Vec2u16 vector);
function Vec2u16 vector2_linear_interpolate(Vec2u16 a, Vec2u16 b, f32 t);
function Vec2u16 vector2_minimum(Vec2u16 a, Vec2u16 b);
function Vec2u16 vector2_maximum(Vec2u16 a, Vec2u16 b);
function Vec2u16 vector2_clamp(Vec2u16 vector, Vec2u16 minimum, Vec2u16 maximum);
function Vec2u16 vector2_absolute(Vec2u16 vector);
function Vec2u16 vector2_perpendicular(Vec2u16 vector);
function Vec2u16 vector2_reflect(Vec2u16 incident, Vec2u16 normal);
function Vec2u16 vector2_project(Vec2u16 vector, Vec2u16 onto);
function Vec2u16 vector2_snap_to_grid(Vec2u16 vector, f32 grid_size);
function f32 vector2_dot_product(Vec2u16 a, Vec2u16 b);
function f32 vector2_cross_product(Vec2u16 a, Vec2u16 b);
function f32 vector2_length(Vec2u16 vector);
function f32 vector2_length_squared(Vec2u16 vector);
function f32 vector2_distance(Vec2u16 a, Vec2u16 b);
function f32 vector2_distance_squared(Vec2u16 a, Vec2u16 b);
function f32 vector2_angle_between(Vec2u16 a, Vec2u16 b);
function f32 vector2_signed_angle_between(Vec2u16 a, Vec2u16 b);

typedef union Vec2u32 { u32 data[2]; struct { u32 x, y; }; } Vec2u32;
function Vec2u32 vector2_add(Vec2u32 a, Vec2u32 b);
function Vec2u32 vector2_subtract(Vec2u32 a, Vec2u32 b);
function Vec2u32 vector2_multiply(Vec2u32 a, Vec2u32 b);
function Vec2u32 vector2_divide(Vec2u32 a, Vec2u32 b);
function Vec2u32 vector2_scale(Vec2u32 vector, f32 scalar);
function Vec2u32 vector2_negate(Vec2u32 vector);
function Vec2u32 vector2_rotate(Vec2u32 vector, f32 radians);
function Vec2u32 vector2_normalize(Vec2u32 vector);
function Vec2u32 vector2_linear_interpolate(Vec2u32 a, Vec2u32 b, f32 t);
function Vec2u32 vector2_minimum(Vec2u32 a, Vec2u32 b);
function Vec2u32 vector2_maximum(Vec2u32 a, Vec2u32 b);
function Vec2u32 vector2_clamp(Vec2u32 vector, Vec2u32 minimum, Vec2u32 maximum);
function Vec2u32 vector2_absolute(Vec2u32 vector);
function Vec2u32 vector2_perpendicular(Vec2u32 vector);
function Vec2u32 vector2_reflect(Vec2u32 incident, Vec2u32 normal);
function Vec2u32 vector2_project(Vec2u32 vector, Vec2u32 onto);
function Vec2u32 vector2_snap_to_grid(Vec2u32 vector, f32 grid_size);
function f32 vector2_dot_product(Vec2u32 a, Vec2u32 b);
function f32 vector2_cross_product(Vec2u32 a, Vec2u32 b);
function f32 vector2_length(Vec2u32 vector);
function f32 vector2_length_squared(Vec2u32 vector);
function f32 vector2_distance(Vec2u32 a, Vec2u32 b);
function f32 vector2_distance_squared(Vec2u32 a, Vec2u32 b);
function f32 vector2_angle_between(Vec2u32 a, Vec2u32 b);
function f32 vector2_signed_angle_between(Vec2u32 a, Vec2u32 b);

typedef union Vec2u64 { u64 data[2]; struct { u64 x, y; }; } Vec2u64;
function Vec2u64 vector2_add(Vec2u64 a, Vec2u64 b);
function Vec2u64 vector2_subtract(Vec2u64 a, Vec2u64 b);
function Vec2u64 vector2_multiply(Vec2u64 a, Vec2u64 b);
function Vec2u64 vector2_divide(Vec2u64 a, Vec2u64 b);
function Vec2u64 vector2_scale(Vec2u64 vector, f32 scalar);
function Vec2u64 vector2_negate(Vec2u64 vector);
function Vec2u64 vector2_rotate(Vec2u64 vector, f32 radians);
function Vec2u64 vector2_normalize(Vec2u64 vector);
function Vec2u64 vector2_linear_interpolate(Vec2u64 a, Vec2u64 b, f32 t);
function Vec2u64 vector2_minimum(Vec2u64 a, Vec2u64 b);
function Vec2u64 vector2_maximum(Vec2u64 a, Vec2u64 b);
function Vec2u64 vector2_clamp(Vec2u64 vector, Vec2u64 minimum, Vec2u64 maximum);
function Vec2u64 vector2_absolute(Vec2u64 vector);
function Vec2u64 vector2_perpendicular(Vec2u64 vector);
function Vec2u64 vector2_reflect(Vec2u64 incident, Vec2u64 normal);
function Vec2u64 vector2_project(Vec2u64 vector, Vec2u64 onto);
function Vec2u64 vector2_snap_to_grid(Vec2u64 vector, f32 grid_size);
function f32 vector2_dot_product(Vec2u64 a, Vec2u64 b);
function f32 vector2_cross_product(Vec2u64 a, Vec2u64 b);
function f32 vector2_length(Vec2u64 vector);
function f32 vector2_length_squared(Vec2u64 vector);
function f32 vector2_distance(Vec2u64 a, Vec2u64 b);
function f32 vector2_distance_squared(Vec2u64 a, Vec2u64 b);
function f32 vector2_angle_between(Vec2u64 a, Vec2u64 b);
function f32 vector2_signed_angle_between(Vec2u64 a, Vec2u64 b);

typedef union Vec2s8  { s8  data[2]; struct { s8  x, y; }; } Vec2s8 ;
function Vec2s8  vector2_add(Vec2s8  a, Vec2s8  b);
function Vec2s8  vector2_subtract(Vec2s8  a, Vec2s8  b);
function Vec2s8  vector2_multiply(Vec2s8  a, Vec2s8  b);
function Vec2s8  vector2_divide(Vec2s8  a, Vec2s8  b);
function Vec2s8  vector2_scale(Vec2s8  vector, f32 scalar);
function Vec2s8  vector2_negate(Vec2s8  vector);
function Vec2s8  vector2_rotate(Vec2s8  vector, f32 radians);
function Vec2s8  vector2_normalize(Vec2s8  vector);
function Vec2s8  vector2_linear_interpolate(Vec2s8  a, Vec2s8  b, f32 t);
function Vec2s8  vector2_minimum(Vec2s8  a, Vec2s8  b);
function Vec2s8  vector2_maximum(Vec2s8  a, Vec2s8  b);
function Vec2s8  vector2_clamp(Vec2s8  vector, Vec2s8  minimum, Vec2s8  maximum);
function Vec2s8  vector2_absolute(Vec2s8  vector);
function Vec2s8  vector2_perpendicular(Vec2s8  vector);
function Vec2s8  vector2_reflect(Vec2s8  incident, Vec2s8  normal);
function Vec2s8  vector2_project(Vec2s8  vector, Vec2s8  onto);
function Vec2s8  vector2_snap_to_grid(Vec2s8  vector, f32 grid_size);
function f32 vector2_dot_product(Vec2s8  a, Vec2s8  b);
function f32 vector2_cross_product(Vec2s8  a, Vec2s8  b);
function f32 vector2_length(Vec2s8  vector);
function f32 vector2_length_squared(Vec2s8  vector);
function f32 vector2_distance(Vec2s8  a, Vec2s8  b);
function f32 vector2_distance_squared(Vec2s8  a, Vec2s8  b);
function f32 vector2_angle_between(Vec2s8  a, Vec2s8  b);
function f32 vector2_signed_angle_between(Vec2s8  a, Vec2s8  b);

typedef union Vec2s16 { s16 data[2]; struct { s16 x, y; }; } Vec2s16;
function Vec2s16 vector2_add(Vec2s16 a, Vec2s16 b);
function Vec2s16 vector2_subtract(Vec2s16 a, Vec2s16 b);
function Vec2s16 vector2_multiply(Vec2s16 a, Vec2s16 b);
function Vec2s16 vector2_divide(Vec2s16 a, Vec2s16 b);
function Vec2s16 vector2_scale(Vec2s16 vector, f32 scalar);
function Vec2s16 vector2_negate(Vec2s16 vector);
function Vec2s16 vector2_rotate(Vec2s16 vector, f32 radians);
function Vec2s16 vector2_normalize(Vec2s16 vector);
function Vec2s16 vector2_linear_interpolate(Vec2s16 a, Vec2s16 b, f32 t);
function Vec2s16 vector2_minimum(Vec2s16 a, Vec2s16 b);
function Vec2s16 vector2_maximum(Vec2s16 a, Vec2s16 b);
function Vec2s16 vector2_clamp(Vec2s16 vector, Vec2s16 minimum, Vec2s16 maximum);
function Vec2s16 vector2_absolute(Vec2s16 vector);
function Vec2s16 vector2_perpendicular(Vec2s16 vector);
function Vec2s16 vector2_reflect(Vec2s16 incident, Vec2s16 normal);
function Vec2s16 vector2_project(Vec2s16 vector, Vec2s16 onto);
function Vec2s16 vector2_snap_to_grid(Vec2s16 vector, f32 grid_size);
function f32 vector2_dot_product(Vec2s16 a, Vec2s16 b);
function f32 vector2_cross_product(Vec2s16 a, Vec2s16 b);
function f32 vector2_length(Vec2s16 vector);
function f32 vector2_length_squared(Vec2s16 vector);
function f32 vector2_distance(Vec2s16 a, Vec2s16 b);
function f32 vector2_distance_squared(Vec2s16 a, Vec2s16 b);
function f32 vector2_angle_between(Vec2s16 a, Vec2s16 b);
function f32 vector2_signed_angle_between(Vec2s16 a, Vec2s16 b);

typedef union Vec2s32 { s32 data[2]; struct { s32 x, y; }; } Vec2s32;
function Vec2s32 vector2_add(Vec2s32 a, Vec2s32 b);
function Vec2s32 vector2_subtract(Vec2s32 a, Vec2s32 b);
function Vec2s32 vector2_multiply(Vec2s32 a, Vec2s32 b);
function Vec2s32 vector2_divide(Vec2s32 a, Vec2s32 b);
function Vec2s32 vector2_scale(Vec2s32 vector, f32 scalar);
function Vec2s32 vector2_negate(Vec2s32 vector);
function Vec2s32 vector2_rotate(Vec2s32 vector, f32 radians);
function Vec2s32 vector2_normalize(Vec2s32 vector);
function Vec2s32 vector2_linear_interpolate(Vec2s32 a, Vec2s32 b, f32 t);
function Vec2s32 vector2_minimum(Vec2s32 a, Vec2s32 b);
function Vec2s32 vector2_maximum(Vec2s32 a, Vec2s32 b);
function Vec2s32 vector2_clamp(Vec2s32 vector, Vec2s32 minimum, Vec2s32 maximum);
function Vec2s32 vector2_absolute(Vec2s32 vector);
function Vec2s32 vector2_perpendicular(Vec2s32 vector);
function Vec2s32 vector2_reflect(Vec2s32 incident, Vec2s32 normal);
function Vec2s32 vector2_project(Vec2s32 vector, Vec2s32 onto);
function Vec2s32 vector2_snap_to_grid(Vec2s32 vector, f32 grid_size);
function f32 vector2_dot_product(Vec2s32 a, Vec2s32 b);
function f32 vector2_cross_product(Vec2s32 a, Vec2s32 b);
function f32 vector2_length(Vec2s32 vector);
function f32 vector2_length_squared(Vec2s32 vector);
function f32 vector2_distance(Vec2s32 a, Vec2s32 b);
function f32 vector2_distance_squared(Vec2s32 a, Vec2s32 b);
function f32 vector2_angle_between(Vec2s32 a, Vec2s32 b);
function f32 vector2_signed_angle_between(Vec2s32 a, Vec2s32 b);

typedef union Vec2s64 { s64 data[2]; struct { s64 x, y; }; } Vec2s64;
function Vec2s64 vector2_add(Vec2s64 a, Vec2s64 b);
function Vec2s64 vector2_subtract(Vec2s64 a, Vec2s64 b);
function Vec2s64 vector2_multiply(Vec2s64 a, Vec2s64 b);
function Vec2s64 vector2_divide(Vec2s64 a, Vec2s64 b);
function Vec2s64 vector2_scale(Vec2s64 vector, f32 scalar);
function Vec2s64 vector2_negate(Vec2s64 vector);
function Vec2s64 vector2_rotate(Vec2s64 vector, f32 radians);
function Vec2s64 vector2_normalize(Vec2s64 vector);
function Vec2s64 vector2_linear_interpolate(Vec2s64 a, Vec2s64 b, f32 t);
function Vec2s64 vector2_minimum(Vec2s64 a, Vec2s64 b);
function Vec2s64 vector2_maximum(Vec2s64 a, Vec2s64 b);
function Vec2s64 vector2_clamp(Vec2s64 vector, Vec2s64 minimum, Vec2s64 maximum);
function Vec2s64 vector2_absolute(Vec2s64 vector);
function Vec2s64 vector2_perpendicular(Vec2s64 vector);
function Vec2s64 vector2_reflect(Vec2s64 incident, Vec2s64 normal);
function Vec2s64 vector2_project(Vec2s64 vector, Vec2s64 onto);
function Vec2s64 vector2_snap_to_grid(Vec2s64 vector, f32 grid_size);
function f32 vector2_dot_product(Vec2s64 a, Vec2s64 b);
function f32 vector2_cross_product(Vec2s64 a, Vec2s64 b);
function f32 vector2_length(Vec2s64 vector);
function f32 vector2_length_squared(Vec2s64 vector);
function f32 vector2_distance(Vec2s64 a, Vec2s64 b);
function f32 vector2_distance_squared(Vec2s64 a, Vec2s64 b);
function f32 vector2_angle_between(Vec2s64 a, Vec2s64 b);
function f32 vector2_signed_angle_between(Vec2s64 a, Vec2s64 b);

typedef union Vec2f32 { f32 data[2]; struct { f32 x, y; }; } Vec2f32;
function Vec2f32 vector2_add(Vec2f32 a, Vec2f32 b);
function Vec2f32 vector2_subtract(Vec2f32 a, Vec2f32 b);
function Vec2f32 vector2_multiply(Vec2f32 a, Vec2f32 b);
function Vec2f32 vector2_divide(Vec2f32 a, Vec2f32 b);
function Vec2f32 vector2_scale(Vec2f32 vector, f32 scalar);
function Vec2f32 vector2_negate(Vec2f32 vector);
function Vec2f32 vector2_rotate(Vec2f32 vector, f32 radians);
function Vec2f32 vector2_normalize(Vec2f32 vector);
function Vec2f32 vector2_linear_interpolate(Vec2f32 a, Vec2f32 b, f32 t);
function Vec2f32 vector2_minimum(Vec2f32 a, Vec2f32 b);
function Vec2f32 vector2_maximum(Vec2f32 a, Vec2f32 b);
function Vec2f32 vector2_clamp(Vec2f32 vector, Vec2f32 minimum, Vec2f32 maximum);
function Vec2f32 vector2_absolute(Vec2f32 vector);
function Vec2f32 vector2_perpendicular(Vec2f32 vector);
function Vec2f32 vector2_reflect(Vec2f32 incident, Vec2f32 normal);
function Vec2f32 vector2_project(Vec2f32 vector, Vec2f32 onto);
function Vec2f32 vector2_snap_to_grid(Vec2f32 vector, f32 grid_size);
function f32 vector2_dot_product(Vec2f32 a, Vec2f32 b);
function f32 vector2_cross_product(Vec2f32 a, Vec2f32 b);
function f32 vector2_length(Vec2f32 vector);
function f32 vector2_length_squared(Vec2f32 vector);
function f32 vector2_distance(Vec2f32 a, Vec2f32 b);
function f32 vector2_distance_squared(Vec2f32 a, Vec2f32 b);
function f32 vector2_angle_between(Vec2f32 a, Vec2f32 b);
function f32 vector2_signed_angle_between(Vec2f32 a, Vec2f32 b);

typedef union Vec2f64 { f64 data[2]; struct { f64 x, y; }; } Vec2f64;
function Vec2f64 vector2_add(Vec2f64 a, Vec2f64 b);
function Vec2f64 vector2_subtract(Vec2f64 a, Vec2f64 b);
function Vec2f64 vector2_multiply(Vec2f64 a, Vec2f64 b);
function Vec2f64 vector2_divide(Vec2f64 a, Vec2f64 b);
function Vec2f64 vector2_scale(Vec2f64 vector, f32 scalar);
function Vec2f64 vector2_negate(Vec2f64 vector);
function Vec2f64 vector2_rotate(Vec2f64 vector, f32 radians);
function Vec2f64 vector2_normalize(Vec2f64 vector);
function Vec2f64 vector2_linear_interpolate(Vec2f64 a, Vec2f64 b, f32 t);
function Vec2f64 vector2_minimum(Vec2f64 a, Vec2f64 b);
function Vec2f64 vector2_maximum(Vec2f64 a, Vec2f64 b);
function Vec2f64 vector2_clamp(Vec2f64 vector, Vec2f64 minimum, Vec2f64 maximum);
function Vec2f64 vector2_absolute(Vec2f64 vector);
function Vec2f64 vector2_perpendicular(Vec2f64 vector);
function Vec2f64 vector2_reflect(Vec2f64 incident, Vec2f64 normal);
function Vec2f64 vector2_project(Vec2f64 vector, Vec2f64 onto);
function Vec2f64 vector2_snap_to_grid(Vec2f64 vector, f32 grid_size);
function f32 vector2_dot_product(Vec2f64 a, Vec2f64 b);
function f32 vector2_cross_product(Vec2f64 a, Vec2f64 b);
function f32 vector2_length(Vec2f64 vector);
function f32 vector2_length_squared(Vec2f64 vector);
function f32 vector2_distance(Vec2f64 a, Vec2f64 b);
function f32 vector2_distance_squared(Vec2f64 a, Vec2f64 b);
function f32 vector2_angle_between(Vec2f64 a, Vec2f64 b);
function f32 vector2_signed_angle_between(Vec2f64 a, Vec2f64 b);

// Vector3
// -------typedef union Vec3u8  { u8  data[3]; struct { u8  x, y, z; }; } Vec3u8 ;
function Vec3u8  vector3_add(Vec3u8  a, Vec3u8  b);
function Vec3u8  vector3_subtract(Vec3u8  a, Vec3u8  b);
function Vec3u8  vector3_multiply(Vec3u8  a, Vec3u8  b);
function Vec3u8  vector3_divide(Vec3u8  a, Vec3u8  b);
function Vec3u8  vector3_scale(Vec3u8  vector, f32 scalar);
function Vec3u8  vector3_negate(Vec3u8  vector);
function Vec3u8  vector3_normalize(Vec3u8  vector);
function Vec3u8  vector3_linear_interpolate(Vec3u8  a, Vec3u8  b, f32 t);
function Vec3u8  vector3_minimum(Vec3u8  a, Vec3u8  b);
function Vec3u8  vector3_maximum(Vec3u8  a, Vec3u8  b);
function Vec3u8  vector3_clamp(Vec3u8  vector, Vec3u8  minimum, Vec3u8  maximum);
function Vec3u8  vector3_absolute(Vec3u8  vector);
function Vec3u8  vector3_reflect(Vec3u8  incident, Vec3u8  normal);
function Vec3u8  vector3_project(Vec3u8  vector, Vec3u8  onto);
function Vec3u8  vector3_snap_to_grid(Vec3u8  vector, f32 grid_size);
function Vec3u8  vector3_cross_product(Vec3u8  a, Vec3u8  b);


function Vec3u8  vector3_rotate_axis_angle(Vec3u8  vector, Vec3u8  axis, f32 radians);
function Vec3u8  vector3_from_to_rotation(Vec3u8  from, Vec3u8  to);
function Vec3u8  vector3_normal_from_triangle(Vec3u8  a, Vec3u8  b, Vec3u8  c);

function f32 vector3_dot_product(Vec3u8  a, Vec3u8  b);
function f32 vector3_length(Vec3u8  vector);
function f32 vector3_length_squared(Vec3u8  vector);
function f32 vector3_distance(Vec3u8  a, Vec3u8  b);
function f32 vector3_distance_squared(Vec3u8  a, Vec3u8  b);
function f32 vector3_angle_between(Vec3u8  a, Vec3u8  b);
function f32 vector3_min_component(Vec3u8  vector);
function f32 vector3_max_component(Vec3u8  vector);


typedef union Vec3u16 { u16 data[3]; struct { u16 x, y, z; }; } Vec3u16;
function Vec3u16 vector3_add(Vec3u16 a, Vec3u16 b);
function Vec3u16 vector3_subtract(Vec3u16 a, Vec3u16 b);
function Vec3u16 vector3_multiply(Vec3u16 a, Vec3u16 b);
function Vec3u16 vector3_divide(Vec3u16 a, Vec3u16 b);
function Vec3u16 vector3_scale(Vec3u16 vector, f32 scalar);
function Vec3u16 vector3_negate(Vec3u16 vector);
function Vec3u16 vector3_normalize(Vec3u16 vector);
function Vec3u16 vector3_linear_interpolate(Vec3u16 a, Vec3u16 b, f32 t);
function Vec3u16 vector3_minimum(Vec3u16 a, Vec3u16 b);
function Vec3u16 vector3_maximum(Vec3u16 a, Vec3u16 b);
function Vec3u16 vector3_clamp(Vec3u16 vector, Vec3u16 minimum, Vec3u16 maximum);
function Vec3u16 vector3_absolute(Vec3u16 vector);
function Vec3u16 vector3_reflect(Vec3u16 incident, Vec3u16 normal);
function Vec3u16 vector3_project(Vec3u16 vector, Vec3u16 onto);
function Vec3u16 vector3_snap_to_grid(Vec3u16 vector, f32 grid_size);
function Vec3u16 vector3_cross_product(Vec3u16 a, Vec3u16 b);


function Vec3u16 vector3_rotate_axis_angle(Vec3u16 vector, Vec3u16 axis, f32 radians);
function Vec3u16 vector3_from_to_rotation(Vec3u16 from, Vec3u16 to);
function Vec3u16 vector3_normal_from_triangle(Vec3u16 a, Vec3u16 b, Vec3u16 c);

function f32 vector3_dot_product(Vec3u16 a, Vec3u16 b);
function f32 vector3_length(Vec3u16 vector);
function f32 vector3_length_squared(Vec3u16 vector);
function f32 vector3_distance(Vec3u16 a, Vec3u16 b);
function f32 vector3_distance_squared(Vec3u16 a, Vec3u16 b);
function f32 vector3_angle_between(Vec3u16 a, Vec3u16 b);
function f32 vector3_min_component(Vec3u16 vector);
function f32 vector3_max_component(Vec3u16 vector);


typedef union Vec3u32 { u32 data[3]; struct { u32 x, y, z; }; } Vec3u32;
function Vec3u32 vector3_add(Vec3u32 a, Vec3u32 b);
function Vec3u32 vector3_subtract(Vec3u32 a, Vec3u32 b);
function Vec3u32 vector3_multiply(Vec3u32 a, Vec3u32 b);
function Vec3u32 vector3_divide(Vec3u32 a, Vec3u32 b);
function Vec3u32 vector3_scale(Vec3u32 vector, f32 scalar);
function Vec3u32 vector3_negate(Vec3u32 vector);
function Vec3u32 vector3_normalize(Vec3u32 vector);
function Vec3u32 vector3_linear_interpolate(Vec3u32 a, Vec3u32 b, f32 t);
function Vec3u32 vector3_minimum(Vec3u32 a, Vec3u32 b);
function Vec3u32 vector3_maximum(Vec3u32 a, Vec3u32 b);
function Vec3u32 vector3_clamp(Vec3u32 vector, Vec3u32 minimum, Vec3u32 maximum);
function Vec3u32 vector3_absolute(Vec3u32 vector);
function Vec3u32 vector3_reflect(Vec3u32 incident, Vec3u32 normal);
function Vec3u32 vector3_project(Vec3u32 vector, Vec3u32 onto);
function Vec3u32 vector3_snap_to_grid(Vec3u32 vector, f32 grid_size);
function Vec3u32 vector3_cross_product(Vec3u32 a, Vec3u32 b);


function Vec3u32 vector3_rotate_axis_angle(Vec3u32 vector, Vec3u32 axis, f32 radians);
function Vec3u32 vector3_from_to_rotation(Vec3u32 from, Vec3u32 to);
function Vec3u32 vector3_normal_from_triangle(Vec3u32 a, Vec3u32 b, Vec3u32 c);

function f32 vector3_dot_product(Vec3u32 a, Vec3u32 b);
function f32 vector3_length(Vec3u32 vector);
function f32 vector3_length_squared(Vec3u32 vector);
function f32 vector3_distance(Vec3u32 a, Vec3u32 b);
function f32 vector3_distance_squared(Vec3u32 a, Vec3u32 b);
function f32 vector3_angle_between(Vec3u32 a, Vec3u32 b);
function f32 vector3_min_component(Vec3u32 vector);
function f32 vector3_max_component(Vec3u32 vector);


typedef union Vec3u64 { u64 data[3]; struct { u64 x, y, z; }; } Vec3u64;
function Vec3u64 vector3_add(Vec3u64 a, Vec3u64 b);
function Vec3u64 vector3_subtract(Vec3u64 a, Vec3u64 b);
function Vec3u64 vector3_multiply(Vec3u64 a, Vec3u64 b);
function Vec3u64 vector3_divide(Vec3u64 a, Vec3u64 b);
function Vec3u64 vector3_scale(Vec3u64 vector, f32 scalar);
function Vec3u64 vector3_negate(Vec3u64 vector);
function Vec3u64 vector3_normalize(Vec3u64 vector);
function Vec3u64 vector3_linear_interpolate(Vec3u64 a, Vec3u64 b, f32 t);
function Vec3u64 vector3_minimum(Vec3u64 a, Vec3u64 b);
function Vec3u64 vector3_maximum(Vec3u64 a, Vec3u64 b);
function Vec3u64 vector3_clamp(Vec3u64 vector, Vec3u64 minimum, Vec3u64 maximum);
function Vec3u64 vector3_absolute(Vec3u64 vector);
function Vec3u64 vector3_reflect(Vec3u64 incident, Vec3u64 normal);
function Vec3u64 vector3_project(Vec3u64 vector, Vec3u64 onto);
function Vec3u64 vector3_snap_to_grid(Vec3u64 vector, f32 grid_size);
function Vec3u64 vector3_cross_product(Vec3u64 a, Vec3u64 b);


function Vec3u64 vector3_rotate_axis_angle(Vec3u64 vector, Vec3u64 axis, f32 radians);
function Vec3u64 vector3_from_to_rotation(Vec3u64 from, Vec3u64 to);
function Vec3u64 vector3_normal_from_triangle(Vec3u64 a, Vec3u64 b, Vec3u64 c);

function f32 vector3_dot_product(Vec3u64 a, Vec3u64 b);
function f32 vector3_length(Vec3u64 vector);
function f32 vector3_length_squared(Vec3u64 vector);
function f32 vector3_distance(Vec3u64 a, Vec3u64 b);
function f32 vector3_distance_squared(Vec3u64 a, Vec3u64 b);
function f32 vector3_angle_between(Vec3u64 a, Vec3u64 b);
function f32 vector3_min_component(Vec3u64 vector);
function f32 vector3_max_component(Vec3u64 vector);


typedef union Vec3s8  { s8  data[3]; struct { s8  x, y, z; }; } Vec3s8 ;
function Vec3s8  vector3_add(Vec3s8  a, Vec3s8  b);
function Vec3s8  vector3_subtract(Vec3s8  a, Vec3s8  b);
function Vec3s8  vector3_multiply(Vec3s8  a, Vec3s8  b);
function Vec3s8  vector3_divide(Vec3s8  a, Vec3s8  b);
function Vec3s8  vector3_scale(Vec3s8  vector, f32 scalar);
function Vec3s8  vector3_negate(Vec3s8  vector);
function Vec3s8  vector3_normalize(Vec3s8  vector);
function Vec3s8  vector3_linear_interpolate(Vec3s8  a, Vec3s8  b, f32 t);
function Vec3s8  vector3_minimum(Vec3s8  a, Vec3s8  b);
function Vec3s8  vector3_maximum(Vec3s8  a, Vec3s8  b);
function Vec3s8  vector3_clamp(Vec3s8  vector, Vec3s8  minimum, Vec3s8  maximum);
function Vec3s8  vector3_absolute(Vec3s8  vector);
function Vec3s8  vector3_reflect(Vec3s8  incident, Vec3s8  normal);
function Vec3s8  vector3_project(Vec3s8  vector, Vec3s8  onto);
function Vec3s8  vector3_snap_to_grid(Vec3s8  vector, f32 grid_size);
function Vec3s8  vector3_cross_product(Vec3s8  a, Vec3s8  b);


function Vec3s8  vector3_rotate_axis_angle(Vec3s8  vector, Vec3s8  axis, f32 radians);
function Vec3s8  vector3_from_to_rotation(Vec3s8  from, Vec3s8  to);
function Vec3s8  vector3_normal_from_triangle(Vec3s8  a, Vec3s8  b, Vec3s8  c);

function f32 vector3_dot_product(Vec3s8  a, Vec3s8  b);
function f32 vector3_length(Vec3s8  vector);
function f32 vector3_length_squared(Vec3s8  vector);
function f32 vector3_distance(Vec3s8  a, Vec3s8  b);
function f32 vector3_distance_squared(Vec3s8  a, Vec3s8  b);
function f32 vector3_angle_between(Vec3s8  a, Vec3s8  b);
function f32 vector3_min_component(Vec3s8  vector);
function f32 vector3_max_component(Vec3s8  vector);


typedef union Vec3s16 { s16 data[3]; struct { s16 x, y, z; }; } Vec3s16;
function Vec3s16 vector3_add(Vec3s16 a, Vec3s16 b);
function Vec3s16 vector3_subtract(Vec3s16 a, Vec3s16 b);
function Vec3s16 vector3_multiply(Vec3s16 a, Vec3s16 b);
function Vec3s16 vector3_divide(Vec3s16 a, Vec3s16 b);
function Vec3s16 vector3_scale(Vec3s16 vector, f32 scalar);
function Vec3s16 vector3_negate(Vec3s16 vector);
function Vec3s16 vector3_normalize(Vec3s16 vector);
function Vec3s16 vector3_linear_interpolate(Vec3s16 a, Vec3s16 b, f32 t);
function Vec3s16 vector3_minimum(Vec3s16 a, Vec3s16 b);
function Vec3s16 vector3_maximum(Vec3s16 a, Vec3s16 b);
function Vec3s16 vector3_clamp(Vec3s16 vector, Vec3s16 minimum, Vec3s16 maximum);
function Vec3s16 vector3_absolute(Vec3s16 vector);
function Vec3s16 vector3_reflect(Vec3s16 incident, Vec3s16 normal);
function Vec3s16 vector3_project(Vec3s16 vector, Vec3s16 onto);
function Vec3s16 vector3_snap_to_grid(Vec3s16 vector, f32 grid_size);
function Vec3s16 vector3_cross_product(Vec3s16 a, Vec3s16 b);


function Vec3s16 vector3_rotate_axis_angle(Vec3s16 vector, Vec3s16 axis, f32 radians);
function Vec3s16 vector3_from_to_rotation(Vec3s16 from, Vec3s16 to);
function Vec3s16 vector3_normal_from_triangle(Vec3s16 a, Vec3s16 b, Vec3s16 c);

function f32 vector3_dot_product(Vec3s16 a, Vec3s16 b);
function f32 vector3_length(Vec3s16 vector);
function f32 vector3_length_squared(Vec3s16 vector);
function f32 vector3_distance(Vec3s16 a, Vec3s16 b);
function f32 vector3_distance_squared(Vec3s16 a, Vec3s16 b);
function f32 vector3_angle_between(Vec3s16 a, Vec3s16 b);
function f32 vector3_min_component(Vec3s16 vector);
function f32 vector3_max_component(Vec3s16 vector);


typedef union Vec3s32 { s32 data[3]; struct { s32 x, y, z; }; } Vec3s32;
function Vec3s32 vector3_add(Vec3s32 a, Vec3s32 b);
function Vec3s32 vector3_subtract(Vec3s32 a, Vec3s32 b);
function Vec3s32 vector3_multiply(Vec3s32 a, Vec3s32 b);
function Vec3s32 vector3_divide(Vec3s32 a, Vec3s32 b);
function Vec3s32 vector3_scale(Vec3s32 vector, f32 scalar);
function Vec3s32 vector3_negate(Vec3s32 vector);
function Vec3s32 vector3_normalize(Vec3s32 vector);
function Vec3s32 vector3_linear_interpolate(Vec3s32 a, Vec3s32 b, f32 t);
function Vec3s32 vector3_minimum(Vec3s32 a, Vec3s32 b);
function Vec3s32 vector3_maximum(Vec3s32 a, Vec3s32 b);
function Vec3s32 vector3_clamp(Vec3s32 vector, Vec3s32 minimum, Vec3s32 maximum);
function Vec3s32 vector3_absolute(Vec3s32 vector);
function Vec3s32 vector3_reflect(Vec3s32 incident, Vec3s32 normal);
function Vec3s32 vector3_project(Vec3s32 vector, Vec3s32 onto);
function Vec3s32 vector3_snap_to_grid(Vec3s32 vector, f32 grid_size);
function Vec3s32 vector3_cross_product(Vec3s32 a, Vec3s32 b);


function Vec3s32 vector3_rotate_axis_angle(Vec3s32 vector, Vec3s32 axis, f32 radians);
function Vec3s32 vector3_from_to_rotation(Vec3s32 from, Vec3s32 to);
function Vec3s32 vector3_normal_from_triangle(Vec3s32 a, Vec3s32 b, Vec3s32 c);

function f32 vector3_dot_product(Vec3s32 a, Vec3s32 b);
function f32 vector3_length(Vec3s32 vector);
function f32 vector3_length_squared(Vec3s32 vector);
function f32 vector3_distance(Vec3s32 a, Vec3s32 b);
function f32 vector3_distance_squared(Vec3s32 a, Vec3s32 b);
function f32 vector3_angle_between(Vec3s32 a, Vec3s32 b);
function f32 vector3_min_component(Vec3s32 vector);
function f32 vector3_max_component(Vec3s32 vector);


typedef union Vec3s64 { s64 data[3]; struct { s64 x, y, z; }; } Vec3s64;
function Vec3s64 vector3_add(Vec3s64 a, Vec3s64 b);
function Vec3s64 vector3_subtract(Vec3s64 a, Vec3s64 b);
function Vec3s64 vector3_multiply(Vec3s64 a, Vec3s64 b);
function Vec3s64 vector3_divide(Vec3s64 a, Vec3s64 b);
function Vec3s64 vector3_scale(Vec3s64 vector, f32 scalar);
function Vec3s64 vector3_negate(Vec3s64 vector);
function Vec3s64 vector3_normalize(Vec3s64 vector);
function Vec3s64 vector3_linear_interpolate(Vec3s64 a, Vec3s64 b, f32 t);
function Vec3s64 vector3_minimum(Vec3s64 a, Vec3s64 b);
function Vec3s64 vector3_maximum(Vec3s64 a, Vec3s64 b);
function Vec3s64 vector3_clamp(Vec3s64 vector, Vec3s64 minimum, Vec3s64 maximum);
function Vec3s64 vector3_absolute(Vec3s64 vector);
function Vec3s64 vector3_reflect(Vec3s64 incident, Vec3s64 normal);
function Vec3s64 vector3_project(Vec3s64 vector, Vec3s64 onto);
function Vec3s64 vector3_snap_to_grid(Vec3s64 vector, f32 grid_size);
function Vec3s64 vector3_cross_product(Vec3s64 a, Vec3s64 b);


function Vec3s64 vector3_rotate_axis_angle(Vec3s64 vector, Vec3s64 axis, f32 radians);
function Vec3s64 vector3_from_to_rotation(Vec3s64 from, Vec3s64 to);
function Vec3s64 vector3_normal_from_triangle(Vec3s64 a, Vec3s64 b, Vec3s64 c);

function f32 vector3_dot_product(Vec3s64 a, Vec3s64 b);
function f32 vector3_length(Vec3s64 vector);
function f32 vector3_length_squared(Vec3s64 vector);
function f32 vector3_distance(Vec3s64 a, Vec3s64 b);
function f32 vector3_distance_squared(Vec3s64 a, Vec3s64 b);
function f32 vector3_angle_between(Vec3s64 a, Vec3s64 b);
function f32 vector3_min_component(Vec3s64 vector);
function f32 vector3_max_component(Vec3s64 vector);


typedef union Vec3f32 { f32 data[3]; struct { f32 x, y, z; }; } Vec3f32;
function Vec3f32 vector3_add(Vec3f32 a, Vec3f32 b);
function Vec3f32 vector3_subtract(Vec3f32 a, Vec3f32 b);
function Vec3f32 vector3_multiply(Vec3f32 a, Vec3f32 b);
function Vec3f32 vector3_divide(Vec3f32 a, Vec3f32 b);
function Vec3f32 vector3_scale(Vec3f32 vector, f32 scalar);
function Vec3f32 vector3_negate(Vec3f32 vector);
function Vec3f32 vector3_normalize(Vec3f32 vector);
function Vec3f32 vector3_linear_interpolate(Vec3f32 a, Vec3f32 b, f32 t);
function Vec3f32 vector3_minimum(Vec3f32 a, Vec3f32 b);
function Vec3f32 vector3_maximum(Vec3f32 a, Vec3f32 b);
function Vec3f32 vector3_clamp(Vec3f32 vector, Vec3f32 minimum, Vec3f32 maximum);
function Vec3f32 vector3_absolute(Vec3f32 vector);
function Vec3f32 vector3_reflect(Vec3f32 incident, Vec3f32 normal);
function Vec3f32 vector3_project(Vec3f32 vector, Vec3f32 onto);
function Vec3f32 vector3_snap_to_grid(Vec3f32 vector, f32 grid_size);
function Vec3f32 vector3_cross_product(Vec3f32 a, Vec3f32 b);


function Vec3f32 vector3_rotate_axis_angle(Vec3f32 vector, Vec3f32 axis, f32 radians);
function Vec3f32 vector3_from_to_rotation(Vec3f32 from, Vec3f32 to);
function Vec3f32 vector3_normal_from_triangle(Vec3f32 a, Vec3f32 b, Vec3f32 c);

function f32 vector3_dot_product(Vec3f32 a, Vec3f32 b);
function f32 vector3_length(Vec3f32 vector);
function f32 vector3_length_squared(Vec3f32 vector);
function f32 vector3_distance(Vec3f32 a, Vec3f32 b);
function f32 vector3_distance_squared(Vec3f32 a, Vec3f32 b);
function f32 vector3_angle_between(Vec3f32 a, Vec3f32 b);
function f32 vector3_min_component(Vec3f32 vector);
function f32 vector3_max_component(Vec3f32 vector);


typedef union Vec3f64 { f64 data[3]; struct { f64 x, y, z; }; } Vec3f64;
function Vec3f64 vector3_add(Vec3f64 a, Vec3f64 b);
function Vec3f64 vector3_subtract(Vec3f64 a, Vec3f64 b);
function Vec3f64 vector3_multiply(Vec3f64 a, Vec3f64 b);
function Vec3f64 vector3_divide(Vec3f64 a, Vec3f64 b);
function Vec3f64 vector3_scale(Vec3f64 vector, f32 scalar);
function Vec3f64 vector3_negate(Vec3f64 vector);
function Vec3f64 vector3_normalize(Vec3f64 vector);
function Vec3f64 vector3_linear_interpolate(Vec3f64 a, Vec3f64 b, f32 t);
function Vec3f64 vector3_minimum(Vec3f64 a, Vec3f64 b);
function Vec3f64 vector3_maximum(Vec3f64 a, Vec3f64 b);
function Vec3f64 vector3_clamp(Vec3f64 vector, Vec3f64 minimum, Vec3f64 maximum);
function Vec3f64 vector3_absolute(Vec3f64 vector);
function Vec3f64 vector3_reflect(Vec3f64 incident, Vec3f64 normal);
function Vec3f64 vector3_project(Vec3f64 vector, Vec3f64 onto);
function Vec3f64 vector3_snap_to_grid(Vec3f64 vector, f32 grid_size);
function Vec3f64 vector3_cross_product(Vec3f64 a, Vec3f64 b);


function Vec3f64 vector3_rotate_axis_angle(Vec3f64 vector, Vec3f64 axis, f32 radians);
function Vec3f64 vector3_from_to_rotation(Vec3f64 from, Vec3f64 to);
function Vec3f64 vector3_normal_from_triangle(Vec3f64 a, Vec3f64 b, Vec3f64 c);

function f32 vector3_dot_product(Vec3f64 a, Vec3f64 b);
function f32 vector3_length(Vec3f64 vector);
function f32 vector3_length_squared(Vec3f64 vector);
function f32 vector3_distance(Vec3f64 a, Vec3f64 b);
function f32 vector3_distance_squared(Vec3f64 a, Vec3f64 b);
function f32 vector3_angle_between(Vec3f64 a, Vec3f64 b);
function f32 vector3_min_component(Vec3f64 vector);
function f32 vector3_max_component(Vec3f64 vector);



#endif // UI_HEPHAESTUS_H// @Hephaestus generated code 2025-October-20 14:06:07

// Forward declare UI structures
typedef struct UI_Node_Color_Scheme UI_Node_Color_Scheme;
typedef struct UI_Color_Scheme UI_Color_Scheme;
typedef struct UI_Node UI_Node;
typedef struct UI_Node_Cache UI_Node_Cache;
typedef struct UI_Signal UI_Signal;
typedef struct UI_Context UI_Context;

// Forward declare UI enums
typedef enum UI_Alignment_Kind UI_Alignment_Kind
typedef enum UI_Width_Kind UI_Width_Kind
typedef enum UI_Height_Kind UI_Height_Kind

typedef u64 UI_Node_Flags;
enum
{
  UI_Node_Flags_Mouse_Clickable = (1 << 0), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Display_Text = (1 << 1), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Draggable = (1 << 2), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Hoverable = (1 << 3), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Center_Text_Horizontally = (1 << 4), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Center_Text_Vertically = (1 << 5), /* TODO(fz): Add documentation */ 
  UI_Node_Flags_Dimensions_Wrap_Text = (1 << 6), /* TODO(fz): Add documentation */ 
};

typedef u64 UI_Node_Flags;
enum
{
  UI_Signal_Flags_Left_Down = (1 << 0), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Middle_Down = (1 << 1), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Right_Down = (1 << 2), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Left_Clicked = (1 << 3), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Middle_Clicked = (1 << 4), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Right_Clicked = (1 << 5), /* TODO(fz): Add documentation */
  UI_Signal_Flags_Mouse_Hovered = (1 << 6), /* TODO(fz): Add documentation */
};

