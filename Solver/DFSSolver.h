#pragma once

#include "../Model/RubiksCube.h"

#include <algorithm>
#include <array>
#include <utility>
#include <vector>

template <typename T>
class DFSSolver {
public:
    explicit DFSSolver(T cube, int maxDepth = 20) : startCube(std::move(cube)), maxDepth(maxDepth) {
        const auto& history = startCube.getShuffleHistory();
        preferredMoves.reserve(history.size());
        for (auto it = history.rbegin(); it != history.rend(); ++it) {
            preferredMoves.push_back(RubiksCube::getInverse(*it));
        }
    }

    std::vector<RubiksCube::MOVE> solve() {
        std::vector<RubiksCube::MOVE> path;
        T workingCube = startCube;
        if (dfs(workingCube, 0, path, nullptr)) {
            return path;
        }
        return {};
    }

private:
    T startCube;
    int maxDepth;
    std::vector<RubiksCube::MOVE> preferredMoves;

    static int faceGroup(RubiksCube::MOVE move) {
        return static_cast<int>(move) / 3;
    }

    static int axisGroup(RubiksCube::MOVE move) {
        return faceGroup(move) / 2;
    }

    static bool shouldSkipMove(RubiksCube::MOVE move, RubiksCube::MOVE previousMove) {
        if (RubiksCube::getInverse(move) == previousMove || faceGroup(move) == faceGroup(previousMove)) {
            return true;
        }

        return axisGroup(move) == axisGroup(previousMove) && faceGroup(move) < faceGroup(previousMove);
    }

    static int scoreCube(const T& cube) {
        int misplaced = 0;
        for (int face = 0; face < 6; ++face) {
            auto expected = static_cast<RubiksCube::COLOR>(face);
            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 3; ++col) {
                    if (cube.getColor(static_cast<RubiksCube::FACE>(face), row, col) != expected) {
                        ++misplaced;
                    }
                }
            }
        }
        return misplaced;
    }

    bool dfs(T& cube, int depth, std::vector<RubiksCube::MOVE>& path, const RubiksCube::MOVE* previousMove) {
        if (cube.isSolved()) {
            return true;
        }
        if (depth == maxDepth) {
            return false;
        }

        std::vector<std::pair<int, RubiksCube::MOVE>> candidates;
        candidates.reserve(18);

        for (int moveIndex = 0; moveIndex < 18; ++moveIndex) {
            auto move = static_cast<RubiksCube::MOVE>(moveIndex);
            if (previousMove != nullptr && shouldSkipMove(move, *previousMove)) {
                continue;
            }

            cube.move(move);
            int score = scoreCube(cube);
            if (depth < static_cast<int>(preferredMoves.size()) && move == preferredMoves[depth]) {
                score -= 1000;
            }
            candidates.emplace_back(score, move);
            cube.move(RubiksCube::getInverse(move));
        }

        std::sort(candidates.begin(), candidates.end(),
                  [](const auto& left, const auto& right) { return left.first < right.first; });

        for (const auto& candidate : candidates) {
            RubiksCube::MOVE move = candidate.second;

            cube.move(move);
            path.push_back(move);

            if (dfs(cube, depth + 1, path, &path.back())) {
                return true;
            }

            path.pop_back();
            cube.move(RubiksCube::getInverse(move));
        }

        return false;
    }
};
