cmake_minimum_required(VERSION 3.20.0)

set(PROBLEM3 "problem3")

add_executable(${PROBLEM3} problem3.cpp)

find_package(OpenMP)

if (OpenMP_CXX_FOUND)
    target_link_libraries(${PROBLEM3} PUBLIC OpenMP::OpenMP_CXX)
endif()

set_target_properties(${PROBLEM3} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../    
)

add_compile_options(-fopenmp)