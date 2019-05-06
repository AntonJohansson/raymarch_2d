cmake_minimum_required(VERSION 3.14)

set(PROJECT raymarch_2d)
set(SOURCES
	src/main.c
	src/perlin.c
	)

project(${PROJECT})
add_executable(${PROJECT} ${SOURCES})

#set(CMAKE_MODULE_PATH "/usr/local/share/SFML/cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
#find_package(SFML 2 COMPONENTS network graphics window system)
find_package(SDL2 REQUIRED) 
find_package(SDL2_gfx REQUIRED) 

target_compile_options(${PROJECT} PRIVATE
	-g
	)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_GFX_INCLUDE_DIRS})

if(UNIX AND NOT APPLE)
	# unix
	target_link_libraries(${PROJECT}
		${SDL2_LIBRARY} ${SDL2_GFX_LIBRARIES} m pthread GL
		)
elseif(APPLE)
	# mac
	target_link_libraries(${PROJECT} ${SDL2_LIBRARY} ${SDL2_GFX_LIBRARIES} m pthread)
endif()
