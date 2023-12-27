
# Set the minimum required version of CMake to be 3.10
cmake_minimum_required(VERSION 3.10)

# Create a project named Tutorial
project(MusSimulator VERSION 1.0)

set(CMAKE_C_COMPILER ${COMPILER_BIN}/gcc CACHE PATH "gcc" )
set(CMAKE_CXX_COMPILER ${COMPILER_BIN}/g++ CACHE PATH "g++" )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

find_package(TBB REQUIRED)

list(APPEND EXTRA_LIBS TBB::tbb)
# Required library: https://github.com/greg7mdp/parallel-hashmap/tree/master
list(APPEND EXTRA_INCLUDES /usr/include/parallel_hashmap)

add_executable(MusSimulator mus_simulator.cpp)

target_link_libraries(MusSimulator PUBLIC ${EXTRA_LIBS})
target_include_directories(MusSimulator PUBLIC ${PROJECT_BINARY_DIR} ${EXTRA_INCLUDES})