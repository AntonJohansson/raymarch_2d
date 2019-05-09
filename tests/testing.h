#pragma once

#define MAX_TEST_FUNCTIONS 16

typedef void(*test_function)(void);

extern void add_test(test_function func);

#define TEST(func)\
	void func(void);\
	add_test(func);\
	void func()
