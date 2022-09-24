cmake_minimum_required(VERSION 3.20.0)

set(PROBLEM2 "problem2")

add_executable(${PROBLEM2} problem2.cpp)

find_package(OpenMP)

if (OpenMP_CXX_FOUND)
    target_link_libraries(${PROBLEM2} PUBLIC OpenMP::OpenMP_CXX)
endif()

set_target_properties(${PROBLEM2} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../    
)

add_compile_options(-fopenmp)