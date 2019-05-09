set(PROJECT tests)
set(SOURCES
	../src/mat.c
	mat_test.c
	)

project(${PROJECT})
add_executable(${PROJECT} ${SOURCES})

target_compile_options(${PROJECT} PRIVATE
	-g
	)

include_directories(../src)
