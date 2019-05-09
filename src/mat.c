#include "mat.h"
#include <stdarg.h>

// matrix memory management
static MAT_VALUE_TYPE* persistent_stack_top;
static MAT_VALUE_TYPE* temporary_stack_top;

MAT_VALUE_TYPE* mat_persistent_memory;
MAT_VALUE_TYPE* mat_temporary_memory;

void mat_allocate_memory(size_t persistent_elements, size_t temporary_elements){
	mat_persistent_memory = calloc(persistent_elements, sizeof(MAT_VALUE_TYPE));
	mat_temporary_memory 	= calloc(temporary_elements, sizeof(MAT_VALUE_TYPE));
	persistent_stack_top = mat_persistent_memory;
	temporary_stack_top = mat_temporary_memory;
}

void mat_free_memory(){
	free(mat_persistent_memory);
	free(mat_temporary_memory);
	mat_persistent_memory = NULL;
	mat_temporary_memory 	= NULL;
	persistent_stack_top 	= NULL;
	temporary_stack_top 	= NULL;
}

void mat_clear_persistent_memory(){
	persistent_stack_top = mat_persistent_memory;
}

void mat_clear_temporary_memory(){
	temporary_stack_top = mat_temporary_memory;
}

MAT_VALUE_TYPE* mat_value_allocate(size_t columns, size_t rows){
	MAT_VALUE_TYPE* ptr = persistent_stack_top;
	persistent_stack_top += columns*rows;
	return ptr;
}

// matrix creation
mat make_mat(MAT_SIZE_TYPE columns, MAT_SIZE_TYPE rows){
	return (mat){columns, rows, mat_value_allocate(columns, rows)};
}

mat make_mat_with(MAT_SIZE_TYPE columns, MAT_SIZE_TYPE rows, ...){
	mat m = make_mat(columns, rows);

	va_list args;
	va_start(args, rows);
	for(MAT_SIZE_TYPE c = 0; c < columns; c++){
		for(MAT_SIZE_TYPE r = 0; r < rows; r++){
			m.data[c*rows + r] = *va_arg(args);
		}
	}
	va_end(args);

	return m;
}
