cmake_minimum_required(VERSION 3.14)

set(PROJECT raymarch_2d)
set(SOURCES
	src/drawgl.c
	src/shader.c
	src/gl_error_checking.c
	third_party/glad/glad.c
	#src/main.c
	#src/perlin.c
	#third_party/cimgui/cimgui.cpp
	)

project(${PROJECT})
add_executable(${PROJECT} ${SOURCES})

#set(CMAKE_MODULE_PATH "/usr/local/share/SFML/cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
#find_package(SFML 2 COMPONENTS network graphics window system)
find_package(SDL2 REQUIRED) 
find_package(SDL2_gfx REQUIRED) 
find_package(OpenGL REQUIRED)

target_compile_options(${PROJECT} PRIVATE
	-g
	)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_GFX_INCLUDE_DIRS} third_party)

if(UNIX AND NOT APPLE)
	# unix
	target_link_libraries(${PROJECT}
		${SDL2_LIBRARY} ${SDL2_GFX_LIBRARIES} dl m pthread GL
		)
elseif(APPLE)
	# mac
	target_link_libraries(${PROJECT} ${SDL2_LIBRARY} ${SDL2_GFX_LIBRARIES} dl m pthread)
endif()
