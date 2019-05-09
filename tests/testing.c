#include "testing.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct test_info{
	test_function test;
	const char* name;
	bool success;
} test_info;

test_info tests[MAX_TEST_FUNCTIONS];
size_t number_of_test_functions = 0;

extern void add_test(test_function func){
	tests[number_of_test_functions++] = (test_info){func, "a", true};
}

int main(){
	for(size_t i = 0; i < number_of_test_functions; i++){
		tests[i].test();
	}
}
