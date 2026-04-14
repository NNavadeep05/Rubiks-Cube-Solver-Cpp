cmake_minimum_required(VERSION 3.15)
project(RubiksCubeSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB MODEL_SRC "Model/*.cpp")
file(GLOB PDBASES_SRC "PatternDatabases/*.cpp")

add_executable(RubiksCubeSolver
    main.cpp
    ${MODEL_SRC}
    ${PDBASES_SRC}
)
