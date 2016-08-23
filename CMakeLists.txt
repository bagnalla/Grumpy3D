cmake_minimum_required (VERSION 2.6)
project (Grumpy3D)

add_compile_options(-std=c++11 -fopenmp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

add_subdirectory(puddi)
add_subdirectory(src)

