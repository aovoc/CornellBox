cmake_minimum_required(VERSION 2.8.4)
project(CornellBox)

if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-narrowing -march=native -m64 -O3 -static -funroll-loops")
endif()

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
	message("OPENMP FOUND")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(
    .
    include
	include/rand48
	include/tiny_obj_loader
    include/glm)
    
    
set(_RESOURCES
		res/scene01.obj
		res/scene01.mtl
		res/scene02.obj
		res/scene02.mtl)

set(_HEADERS
		include/BBox.h
		include/BVH.h
		include/Camera.h
		include/glm_vec3_func.h
		include/IntersectionInfo.h
		include/Material.h
		include/Object.h
		include/Ray.h
		include/Renderer.h
		include/Scene.h
		include/Sphere.h
		include/Triangle.h
		include/tiny_obj_loader/tiny_obj_loader.h
		include/rand48/erand48.inc
    include/tiny_obj_loader/tiny_obj_loader.cc)

set(SOURCE_FILES
		src/BBox.cpp
		src/BVH.cpp
		src/Camera.cpp
		src/main.cpp
		src/Renderer.cpp
		src/Scene.cpp)
    
    

add_executable(
		CornellBox ${SOURCE_FILES}
		${_HEADERS})