cmake_minimum_required(VERSION 3.20.0)

set(PROBLEM "problem5")

add_executable(${PROBLEM} problem5.cpp)

find_package(OpenMP)

if (OpenMP_CXX_FOUND)
    target_link_libraries(${PROBLEM} PUBLIC OpenMP::OpenMP_CXX)
endif()

set_target_properties(${PROBLEM} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../    
)

add_compile_options(-fopenmp)