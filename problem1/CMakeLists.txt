cmake_minimum_required(VERSION 3.20.0)

set(PROBLEM1 "problem1")
set(PROBLEM1_2 "problem1_2")

add_executable(${PROBLEM1} problem1.cpp)
add_executable(${PROBLEM1_2} problem1_2.cpp)

find_package(OpenMP)

if (OpenMP_CXX_FOUND)
    target_link_libraries(${PROBLEM1} PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(${PROBLEM1_2} PUBLIC OpenMP::OpenMP_CXX)
endif()

set_target_properties(${PROBLEM1} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../    
)

set_target_properties(${PROBLEM1_2} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../    
)

add_compile_options(-fopenmp)