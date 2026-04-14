# Rubik's Cube Solver | C++
A C++17 Rubik's Cube solver featuring multiple cube representations and classical graph search algorithms.

## Architecture
The project is organized around an abstract `RubiksCube` base class that defines the cube interface, move set, and shared utilities. Three concrete models implement the same behavior with different storage layouts: a readable 3D array, a linear 1D array, and a compact bitboard. On top of that interface, four templated solvers operate generically across any cube model without changing solver logic.

## Cube Representations

| Model | Storage | Notes |
|---|---|---|
| 3D Array | `char[6][3][3]` | Most readable |
| 1D Array | `char[54]` | Linearised, cache friendly |
| Bitboard | `uint32_t[6]` | Most compact, bitwise ops |

## Move Notation

| Move | Meaning |
|---|---|
| F, F', F2 | Front face CW / CCW / 180 degree |
| R, R', R2 | Right face CW / CCW / 180 degree |
| U, U', U2 | Up face CW / CCW / 180 degree |
| L, L', L2 | Left face CW / CCW / 180 degree |
| D, D', D2 | Down face CW / CCW / 180 degree |
| B, B', B2 | Back face CW / CCW / 180 degree |

## Solvers

| Solver | Strategy | Practical shuffle depth |
|---|---|---|
| DFS | Depth-capped DFS, no visited map | <= 8 moves |
| IDDFS | Iterative deepening, optimal path | <= 8 moves |
| BFS | Level-by-level, shortest solution | <= 5 moves |
| IDA* | IDA* with corner heuristic | <= 20 moves |

## Folder Structure

```text
rubiks-cube-solver/
|-- Model/
|   |-- RubiksCube.h
|   |-- RubiksCube.cpp
|   |-- RubiksCube3dArray.h
|   |-- RubiksCube3dArray.cpp
|   |-- RubiksCube1dArray.h
|   |-- RubiksCube1dArray.cpp
|   |-- RubiksCubeBitboard.h
|   `-- RubiksCubeBitboard.cpp
|-- Solver/
|   |-- DFSSolver.h
|   |-- BFSSolver.h
|   |-- IDDFSSolver.h
|   `-- IDAstarSolver.h
|-- PatternDatabases/
|   |-- PatternDatabase.h
|   |-- CornerPatternDatabase.h
|   |-- CornerPatternDatabase.cpp
|   `-- CornerDBMaker.h
|-- Databases/
|   `-- (empty - populated at runtime by CornerDBMaker)
|-- main.cpp
|-- CMakeLists.txt
`-- README.md
```

## Build & Run

```bash
git clone <repo-url>
cd rubiks-cube-solver
mkdir build && cd build
cmake ..
make
./RubiksCubeSolver
```

## Developer
- Navadeep Nandedapu | Indian Institute of Technology Kharagpur
