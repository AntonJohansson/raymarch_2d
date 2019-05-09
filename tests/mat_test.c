#include <mat.h>
#include <stdio.h>

int main(){
	mat_allocate_memory(16, 16);
	mat m;
	m = make_mat(4, 4);
	mat_print(m);


	printf("\n");
	m = make_mat_with(4, 4,
										1.0, 	2.0, 	3.0, 	4.0,
										5.0, 	6.0, 	7.0, 	8.0,
										9.0, 	10.0, 11.0, 12.0,
										13.0, 14.0, 15.0, 16.0
										);
	mat_print(m);

	printf("\n");
	m = negate(m);
	mat_print(m);

	printf("\n");
	mat_print(row(m, 1));

	mat_free_memory();
}
