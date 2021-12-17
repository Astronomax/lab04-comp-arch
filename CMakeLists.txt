cmake_minimum_required(VERSION 3.17)
project(lab04_acs)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
add_executable(lab04_acs main.cpp include/str_switch.h src/P6_pnm_img.cpp include/P6_pnm_img.h)