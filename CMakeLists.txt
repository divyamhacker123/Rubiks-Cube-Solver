cmake_minimum_required(VERSION 3.10)
project(RubiksCubeSolver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Core library containing the cube state and moves
add_library(CubeCore src/RubiksCube.cpp src/PatternDatabase.cpp)
target_include_directories(CubeCore PUBLIC include)

# Executable 1: Generates the Pattern Databases
add_executable(DBGenerator src/generator_main.cpp)
target_link_libraries(DBGenerator CubeCore)

# Executable 2: The actual IDA* Solver
add_executable(Solver src/solver_main.cpp)
target_link_libraries(Solver CubeCore)