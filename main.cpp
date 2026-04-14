#include "Model/RubiksCube3dArray.h"
#include "Solver/BFSSolver.h"
#include "Solver/DFSSolver.h"
#include "Solver/IDAstarSolver.h"
#include "Solver/IDDFSSolver.h"

#include <iostream>
#include <string>
#include <vector>

namespace {
void printSeparator(const std::string& title) {
    std::cout << "\n==================== " << title << " ====================\n";
}

void printMoves(const std::vector<RubiksCube::MOVE>& moves) {
    if (moves.empty()) {
        std::cout << "No solution found.\n";
        return;
    }

    std::cout << "Solution: ";
    for (RubiksCube::MOVE move : moves) {
        std::cout << RubiksCube::moveToString(move) << ' ';
    }
    std::cout << '\n';
}

void applySolution(RubiksCube3dArray& cube, const std::vector<RubiksCube::MOVE>& moves) {
    for (RubiksCube::MOVE move : moves) {
        cube.move(move);
    }
}
}

int main() {
    {
        printSeparator("DFS Test");
        RubiksCube3dArray cube;
        cube.shuffle(7);
        std::cout << "Shuffled cube:\n";
        cube.print();

        DFSSolver<RubiksCube3dArray> solver(cube, 8);
        auto solution = solver.solve();
        printMoves(solution);
        applySolution(cube, solution);
        std::cout << "Final cube:\n";
        cube.print();
        std::cout << "Move count: " << solution.size() << '\n';
    }

    {
        printSeparator("IDDFS Test");
        RubiksCube3dArray cube;
        cube.shuffle(7);
        std::cout << "Shuffled cube:\n";
        cube.print();

        IDDFSSolver<RubiksCube3dArray> solver(cube, 8);
        auto solution = solver.solve();
        printMoves(solution);
        std::cout << "Move count: " << solution.size() << '\n';
    }

    {
        printSeparator("BFS Test");
        RubiksCube3dArray cube;
        cube.shuffle(5);
        std::cout << "Shuffled cube:\n";
        cube.print();

        BFSSolver<RubiksCube3dArray> solver(cube);
        auto solution = solver.solve();
        printMoves(solution);
        std::cout << "Move count: " << solution.size() << '\n';
    }

    {
        printSeparator("IDA* Test");
        RubiksCube3dArray cube;
        cube.shuffle(7);
        std::cout << "Shuffled cube:\n";
        cube.print();

        IDAstarSolver<RubiksCube3dArray> solver(cube);
        auto solution = solver.solve();
        printMoves(solution);
        std::cout << "Move count: " << solution.size() << '\n';
    }

    return 0;
}
