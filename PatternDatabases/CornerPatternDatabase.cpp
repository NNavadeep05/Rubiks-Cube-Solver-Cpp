#include "CornerPatternDatabase.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace {
struct StickerRef {
    RubiksCube::FACE face;
    int row;
    int col;
};

struct CornerDefinition {
    std::array<StickerRef, 3> stickers;
    std::array<RubiksCube::COLOR, 3> solvedColors;
};

CornerDefinition makeCorner(int x, int y, int z) {
    std::array<StickerRef, 3> stickers{};
    std::array<RubiksCube::COLOR, 3> colors{};
    const std::array<std::pair<int, int>, 3> axes = {{{x, 0}, {y, 1}, {z, 2}}};

    for (int index = 0; index < 3; ++index) {
        int sign = axes[index].first;
        int axis = axes[index].second;

        if (axis == 0) {
            if (sign == 1) {
                stickers[index] = {RubiksCube::FACE::RIGHT, 1 - y, 1 - z};
                colors[index] = RubiksCube::COLOR::BLUE;
            } else {
                stickers[index] = {RubiksCube::FACE::LEFT, 1 - y, z + 1};
                colors[index] = RubiksCube::COLOR::GREEN;
            }
        } else if (axis == 1) {
            if (sign == 1) {
                stickers[index] = {RubiksCube::FACE::UP, z + 1, x + 1};
                colors[index] = RubiksCube::COLOR::WHITE;
            } else {
                stickers[index] = {RubiksCube::FACE::DOWN, 1 - z, x + 1};
                colors[index] = RubiksCube::COLOR::YELLOW;
            }
        } else {
            if (sign == 1) {
                stickers[index] = {RubiksCube::FACE::FRONT, 1 - y, x + 1};
                colors[index] = RubiksCube::COLOR::RED;
            } else {
                stickers[index] = {RubiksCube::FACE::BACK, 1 - y, 1 - x};
                colors[index] = RubiksCube::COLOR::ORANGE;
            }
        }
    }

    return {stickers, colors};
}

const std::array<CornerDefinition, 8>& getCornerDefinitions() {
    static const std::array<CornerDefinition, 8> corners = {
        makeCorner(1, 1, 1), makeCorner(1, 1, -1), makeCorner(-1, 1, -1), makeCorner(-1, 1, 1),
        makeCorner(1, -1, 1), makeCorner(1, -1, -1), makeCorner(-1, -1, -1), makeCorner(-1, -1, 1)
    };
    return corners;
}

std::array<int, 3> sortedColors(const std::array<RubiksCube::COLOR, 3>& colors) {
    std::array<int, 3> values = {
        static_cast<int>(colors[0]),
        static_cast<int>(colors[1]),
        static_cast<int>(colors[2])
    };
    std::sort(values.begin(), values.end());
    return values;
}

int permutationRank(const std::array<int, 8>& permutation) {
    static const std::array<int, 8> factorial = {5040, 720, 120, 24, 6, 2, 1, 1};

    int rank = 0;
    for (int i = 0; i < 8; ++i) {
        int smallerUnused = 0;
        for (int j = i + 1; j < 8; ++j) {
            if (permutation[j] < permutation[i]) {
                ++smallerUnused;
            }
        }
        rank += smallerUnused * factorial[i];
    }
    return rank;
}
}

CornerPatternDatabase::CornerPatternDatabase() {
    database.assign(88179840U, std::numeric_limits<uint8_t>::max());
}

uint8_t CornerPatternDatabase::getNumMoves(const RubiksCube3dArray& cube) const {
    return database[encode(cube)];
}

uint32_t CornerPatternDatabase::encode(const RubiksCube3dArray& cube) const {
    const auto& corners = getCornerDefinitions();

    std::array<std::array<int, 3>, 8> solvedCornerSets{};
    for (int i = 0; i < 8; ++i) {
        solvedCornerSets[i] = sortedColors(corners[i].solvedColors);
    }

    std::array<int, 8> permutation{};
    std::array<int, 8> orientation{};

    for (int position = 0; position < 8; ++position) {
        std::array<RubiksCube::COLOR, 3> actualColors = {
            cube.getColor(corners[position].stickers[0].face, corners[position].stickers[0].row, corners[position].stickers[0].col),
            cube.getColor(corners[position].stickers[1].face, corners[position].stickers[1].row, corners[position].stickers[1].col),
            cube.getColor(corners[position].stickers[2].face, corners[position].stickers[2].row, corners[position].stickers[2].col)
        };

        std::array<int, 3> actualSet = {
            static_cast<int>(actualColors[0]),
            static_cast<int>(actualColors[1]),
            static_cast<int>(actualColors[2])
        };
        std::sort(actualSet.begin(), actualSet.end());

        int cubieId = -1;
        for (int candidate = 0; candidate < 8; ++candidate) {
            if (actualSet == solvedCornerSets[candidate]) {
                cubieId = candidate;
                break;
            }
        }
        if (cubieId == -1) {
            throw std::runtime_error("Invalid corner configuration");
        }

        permutation[position] = cubieId;

        RubiksCube::COLOR upDownColor = corners[cubieId].solvedColors[0];
        for (RubiksCube::COLOR color : corners[cubieId].solvedColors) {
            if (color == RubiksCube::COLOR::WHITE || color == RubiksCube::COLOR::YELLOW) {
                upDownColor = color;
                break;
            }
        }

        int upDownSticker = 0;
        for (int i = 0; i < 3; ++i) {
            if (actualColors[i] == upDownColor) {
                upDownSticker = i;
                break;
            }
        }

        RubiksCube::FACE face = corners[position].stickers[upDownSticker].face;
        if (face == RubiksCube::FACE::UP || face == RubiksCube::FACE::DOWN) {
            orientation[position] = 0;
        } else if (face == RubiksCube::FACE::LEFT || face == RubiksCube::FACE::RIGHT) {
            orientation[position] = 1;
        } else {
            orientation[position] = 2;
        }
    }

    int orientationRank = 0;
    int multiplier = 1;
    for (int i = 0; i < 7; ++i) {
        orientationRank += orientation[i] * multiplier;
        multiplier *= 3;
    }

    return static_cast<uint32_t>(permutationRank(permutation) * 2187 + orientationRank);
}
