#pragma once

#include <stdint.h>
#include <stddef.h>

#define MAT_VALUE_TYPE double
#define MAT_SIZE_TYPE uint8_t

typedef struct mat{
	MAT_SIZE_TYPE columns, rows;
	MAT_VALUE_TYPE* data;
} mat;

//  Memory management
extern MAT_VALUE_TYPE* mat_persistent_memory;
extern MAT_VALUE_TYPE* mat_temporary_memory;

extern void mat_allocate_memory(size_t persistent_elements, size_t temporary_elements);
extern void mat_free_memory();
extern void mat_clear_persistent_memory();
extern void mat_clear_temporary_memory();

extern MAT_VALUE_TYPE* mat_value_allocate(size_t columns, size_t rows);

// matrix creation
extern mat make_mat(MAT_SIZE_TYPE columns, MAT_SIZE_TYPE rows);
extern mat make_mat_with(MAT_SIZE_TYPE columns, MAT_SIZE_TYPE rows, ...);

// matrix operation
//inline vec2 negate(vec2 a){
//	vec2 v = {.x = -a.x, .y = -a.y};
//	return v;
//}
//
//inline vec2 add(vec2 a, vec2 b){
//	vec2 v = {.x = a.x + b.x, .y = a.y + b.y};
//	return v;
//}
//
//inline vec2 sub(vec2 a, vec2 b){
//	return add(a,negate(b));
//}
//
//inline vec2 mul(double f, vec2 a){
//	vec2 v = {.x = f*a.x, .y = f*a.y};
//	return v;
//}
//
//inline double length(vec2 a){
//	return sqrt(a.x*a.x + a.y*a.y);
//}
//
//inline vec2 normalize(vec2 a){
//	double l = length(a);
//	vec2 v = {.x = a.x/l, .y = a.y/l};
//	return v;
//}
//
//
//
