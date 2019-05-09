#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#define MAT_VALUE_TYPE double
#define MAT_SIZE_TYPE uint8_t

typedef struct mat{
	MAT_SIZE_TYPE columns, rows;
	MAT_VALUE_TYPE* data;
} mat;

//  Memory management
extern MAT_VALUE_TYPE* mat_persistent_memory;
extern MAT_VALUE_TYPE* mat_temporary_memory;
extern mat temp;

extern void mat_allocate_memory(size_t persistent_elements, size_t temporary_elements);
extern void mat_free_memory();
extern void mat_clear_persistent_memory();
extern void mat_clear_temporary_memory();

typedef enum mat_allocate_mode{
	PERSISTENT,
	TEMPORARY
} mat_allocate_mode;

extern void mat_memory(mat_allocate_mode mode);

extern MAT_VALUE_TYPE* mat_value_allocate(size_t columns, size_t rows);

// matrix creation
extern mat make_mat(MAT_SIZE_TYPE columns, MAT_SIZE_TYPE rows);
extern mat make_mat_with(MAT_SIZE_TYPE columns, MAT_SIZE_TYPE rows, ...);

static void mat_print(mat m){
	for(MAT_SIZE_TYPE r = 0; r < m.rows; r++){
		for(MAT_SIZE_TYPE c = 0; c < m.columns; c++){
			printf("%lf ", (double) m.data[c*m.rows + r]);
		}
		printf("\n");
	}
}

// matrix operation
static mat negate(mat m){
	assert(m.columns <= temp.columns && m.rows <= temp.rows);
	for(MAT_SIZE_TYPE i = 0; i < m.columns*m.rows; i++){
		temp.data[i] = -m.data[i];
	}

	return (mat){m.columns, m.rows, temp.data};
}

static mat row(mat m, MAT_SIZE_TYPE r){
	assert(r <= m.rows);
	assert(m.columns <= temp.columns && m.rows <= temp.rows);
	for(MAT_SIZE_TYPE c = 0; c < m.columns; c++){
		temp.data[c] = m.data[c*m.rows + r];
	}
	return (mat){m.columns, 1, &temp.data[0]};
}

static mat col(mat m, MAT_SIZE_TYPE c){
	assert(c <= m.columns);
	return (mat){1, m.rows, &m.data[c*m.rows]};
}
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
