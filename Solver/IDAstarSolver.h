#pragma once

#include "../Model/RubiksCube.h"

#include <algorithm>
#include <array>
#include <limits>
#include <utility>
#include <vector>

template <typename T>
class IDAstarSolver {
public:
    explicit IDAstarSolver(T cube) : startCube(std::move(cube)) {}

    std::vector<RubiksCube::MOVE> solve() {
        std::vector<RubiksCube::MOVE> path;
        T workingCube = startCube;
        int threshold = heuristic(workingCube);

        while (true) {
            int nextThreshold = search(workingCube, 0, threshold, path, nullptr);
            if (nextThreshold == solvedSentinel()) {
                return path;
            }
            if (nextThreshold == std::numeric_limits<int>::max()) {
                return {};
            }
            threshold = nextThreshold;
        }
    }

private:
    struct StickerRef {
        RubiksCube::FACE face;
        int row;
        int col;
    };

    struct CornerDescriptor {
        std::array<StickerRef, 3> stickers;
        std::array<RubiksCube::COLOR, 3> solvedColors;
    };

    T startCube;

    static int solvedSentinel() {
        return -1;
    }

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

    static CornerDescriptor makeCorner(int x, int y, int z) {
        std::array<StickerRef, 3> stickers{};
        std::array<RubiksCube::COLOR, 3> solvedColors{};
        const std::array<std::pair<int, int>, 3> axes = {{{x, 0}, {y, 1}, {z, 2}}};

        for (int index = 0; index < 3; ++index) {
            int sign = axes[index].first;
            int axis = axes[index].second;

            if (axis == 0) {
                if (sign == 1) {
                    stickers[index] = {RubiksCube::FACE::RIGHT, 1 - y, 1 - z};
                    solvedColors[index] = RubiksCube::COLOR::BLUE;
                } else {
                    stickers[index] = {RubiksCube::FACE::LEFT, 1 - y, z + 1};
                    solvedColors[index] = RubiksCube::COLOR::GREEN;
                }
            } else if (axis == 1) {
                if (sign == 1) {
                    stickers[index] = {RubiksCube::FACE::UP, z + 1, x + 1};
                    solvedColors[index] = RubiksCube::COLOR::WHITE;
                } else {
                    stickers[index] = {RubiksCube::FACE::DOWN, 1 - z, x + 1};
                    solvedColors[index] = RubiksCube::COLOR::YELLOW;
                }
            } else {
                if (sign == 1) {
                    stickers[index] = {RubiksCube::FACE::FRONT, 1 - y, x + 1};
                    solvedColors[index] = RubiksCube::COLOR::RED;
                } else {
                    stickers[index] = {RubiksCube::FACE::BACK, 1 - y, 1 - x};
                    solvedColors[index] = RubiksCube::COLOR::ORANGE;
                }
            }
        }

        return {stickers, solvedColors};
    }

    static const std::array<CornerDescriptor, 8>& corners() {
        static const std::array<CornerDescriptor, 8> data = {
            makeCorner(1, 1, 1), makeCorner(1, 1, -1), makeCorner(-1, 1, -1), makeCorner(-1, 1, 1),
            makeCorner(1, -1, 1), makeCorner(1, -1, -1), makeCorner(-1, -1, -1), makeCorner(-1, -1, 1)
        };
        return data;
    }

    static int heuristic(const T& cube) {
        int misplacedCorners = 0;
        for (const auto& corner : corners()) {
            if (cube.getColor(corner.stickers[0].face, corner.stickers[0].row, corner.stickers[0].col) != corner.solvedColors[0] ||
                cube.getColor(corner.stickers[1].face, corner.stickers[1].row, corner.stickers[1].col) != corner.solvedColors[1] ||
                cube.getColor(corner.stickers[2].face, corner.stickers[2].row, corner.stickers[2].col) != corner.solvedColors[2]) {
                ++misplacedCorners;
            }
        }
        return (misplacedCorners + 3) / 4;
    }

    int search(T& cube, int g, int threshold, std::vector<RubiksCube::MOVE>& path,
               const RubiksCube::MOVE* previousMove) {
        int h = heuristic(cube);
        int f = g + h;
        if (f > threshold) {
            return f;
        }
        if (cube.isSolved()) {
            return solvedSentinel();
        }

        int minimumOverflow = std::numeric_limits<int>::max();

        std::vector<std::pair<int, RubiksCube::MOVE>> candidates;
        candidates.reserve(18);

        for (int moveIndex = 0; moveIndex < 18; ++moveIndex) {
            auto move = static_cast<RubiksCube::MOVE>(moveIndex);
            if (previousMove != nullptr && shouldSkipMove(move, *previousMove)) {
                continue;
            }

            cube.move(move);
            candidates.emplace_back(heuristic(cube), move);
            cube.move(RubiksCube::getInverse(move));
        }

        std::sort(candidates.begin(), candidates.end(),
                  [](const auto& left, const auto& right) { return left.first < right.first; });

        for (const auto& candidate : candidates) {
            RubiksCube::MOVE move = candidate.second;

            cube.move(move);
            path.push_back(move);

            int result = search(cube, g + 1, threshold, path, &path.back());
            if (result == solvedSentinel()) {
                return solvedSentinel();
            }
            minimumOverflow = std::min(minimumOverflow, result);

            path.pop_back();
            cube.move(RubiksCube::getInverse(move));
        }

        return minimumOverflow;
    }
};
