#pragma once

#include "../Model/RubiksCube.h"

#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T>
class BFSSolver {
public:
    explicit BFSSolver(T cube) : startCube(std::move(cube)) {}

    std::vector<RubiksCube::MOVE> solve() {
        std::queue<T> frontier;
        std::unordered_map<T, std::vector<RubiksCube::MOVE>, typename T::Hash> visited;

        frontier.push(startCube);
        visited.emplace(startCube, std::vector<RubiksCube::MOVE>{});

        while (!frontier.empty()) {
            T current = frontier.front();
            frontier.pop();

            const auto currentPath = visited.at(current);
            if (current.isSolved()) {
                return currentPath;
            }

            for (int moveIndex = 0; moveIndex < 18; ++moveIndex) {
                auto move = static_cast<RubiksCube::MOVE>(moveIndex);
                if (!currentPath.empty()) {
                    RubiksCube::MOVE previousMove = currentPath.back();
                    if (shouldSkipMove(move, previousMove)) {
                        continue;
                    }
                }

                T next = current;
                next.move(move);

                if (visited.find(next) != visited.end()) {
                    continue;
                }

                auto nextPath = currentPath;
                nextPath.push_back(move);
                visited.emplace(next, std::move(nextPath));
                frontier.push(next);
            }
        }

        return {};
    }

private:
    T startCube;

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
};
