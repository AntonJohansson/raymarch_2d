set(PROJECT tests)
set(SOURCES
	testing.c
	mat_test.c

	../src/mat.c
	)

project(${PROJECT})
add_executable(${PROJECT} ${SOURCES})

target_compile_options(${PROJECT} PRIVATE
	-g
	)

include_directories(../src)
