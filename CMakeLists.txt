cmake_minimum_required(VERSION 3.17)
project(BreakingCycles)

set(CMAKE_CXX_STANDARD 17)

add_custom_target(-g -o MFVS/heuristic/MakeMinimalWithSorting.cpp)

add_executable(BreakingCycles main.cpp)
add_executable(ExactMain ExactMain.cpp)
add_executable(HeuristicMain HeuristicMain.cpp)
add_executable(TimesComparison TimesComparison.cpp)
