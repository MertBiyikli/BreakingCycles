cmake_minimum_required(VERSION 3.17)
project(BreakingCycles)

set(CMAKE_CXX_STANDARD 17)

include_directories(MFVS/include)
include_directories(MFVS/heuristic)
include_directories(MFVS/exact)
include_directories(MFVS/io)
include_directories(MFVS/algorithms)

add_custom_target(g++ -g -o MFVS/include/graph.cpp MFVS/io/graph_io.cpp)

add_executable(BreakingCycles main.cpp)
add_executable(ExactMain ExactMain.cpp)
add_executable(HeuristicMain HeuristicMain.cpp)
add_executable(TimesComparison TimesComparison.cpp)
