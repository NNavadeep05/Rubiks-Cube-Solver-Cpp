#include "RubiksCube3dArray.h"

#include <cstring>
#include <iostream>

namespace {
void rotateFaceClockwise(char face[3][3]) {
    char copy[3][3];
    std::memcpy(copy, face, sizeof(copy));

    for (int col = 0; col < 3; ++col) {
        face[0][col] = copy[2 - col][0];
        face[col][2] = copy[0][col];
        face[2][2 - col] = copy[col][2];
        face[2 - col][0] = copy[2][2 - col];
    }
}
}

RubiksCube3dArray::RubiksCube3dArray() {
    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                cube[face][row][col] = static_cast<char>(face);
            }
        }
    }
}

RubiksCube::COLOR RubiksCube3dArray::getColor(FACE face, int row, int col) const {
    return static_cast<COLOR>(cube[static_cast<int>(face)][row][col]);
}

bool RubiksCube3dArray::isSolved() const {
    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (cube[face][row][col] != static_cast<char>(face)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void RubiksCube3dArray::print() const {
    for (int row = 0; row < 3; ++row) {
        std::cout << "      ";
        for (int col = 0; col < 3; ++col) {
            std::cout << getColor(FACE::UP, row, col) << ' ';
        }
        std::cout << '\n';
    }

    for (int row = 0; row < 3; ++row) {
        for (FACE face : {FACE::LEFT, FACE::FRONT, FACE::RIGHT, FACE::BACK}) {
            for (int col = 0; col < 3; ++col) {
                std::cout << getColor(face, row, col) << ' ';
            }
            std::cout << "  ";
        }
        std::cout << '\n';
    }

    for (int row = 0; row < 3; ++row) {
        std::cout << "      ";
        for (int col = 0; col < 3; ++col) {
            std::cout << getColor(FACE::DOWN, row, col) << ' ';
        }
        std::cout << '\n';
    }
}

void RubiksCube3dArray::applyTurn(FACE face, int quarterTurns) {
    quarterTurns = ((quarterTurns % 4) + 4) % 4;
    while (quarterTurns-- > 0) {
        switch (face) {
            case FACE::UP: {
                rotateFaceClockwise(cube[0]);
                char temp[3];
                for (int i = 0; i < 3; ++i) {
                    temp[i] = cube[4][0][2 - i];
                }
                for (int i = 0; i < 3; ++i) cube[4][0][2 - i] = cube[1][0][2 - i];
                for (int i = 0; i < 3; ++i) cube[1][0][2 - i] = cube[2][0][2 - i];
                for (int i = 0; i < 3; ++i) cube[2][0][2 - i] = cube[3][0][2 - i];
                for (int i = 0; i < 3; ++i) cube[3][0][2 - i] = temp[i];
                break;
            }
            case FACE::LEFT: {
                rotateFaceClockwise(cube[1]);
                char temp[3];
                for (int i = 0; i < 3; ++i) temp[i] = cube[0][i][0];
                for (int i = 0; i < 3; ++i) cube[0][i][0] = cube[4][2 - i][2];
                for (int i = 0; i < 3; ++i) cube[4][2 - i][2] = cube[5][i][0];
                for (int i = 0; i < 3; ++i) cube[5][i][0] = cube[2][i][0];
                for (int i = 0; i < 3; ++i) cube[2][i][0] = temp[i];
                break;
            }
            case FACE::FRONT: {
                rotateFaceClockwise(cube[2]);
                char temp[3];
                for (int i = 0; i < 3; ++i) temp[i] = cube[0][2][i];
                for (int i = 0; i < 3; ++i) cube[0][2][i] = cube[1][2 - i][2];
                for (int i = 0; i < 3; ++i) cube[1][2 - i][2] = cube[5][0][2 - i];
                for (int i = 0; i < 3; ++i) cube[5][0][2 - i] = cube[3][i][0];
                for (int i = 0; i < 3; ++i) cube[3][i][0] = temp[i];
                break;
            }
            case FACE::RIGHT: {
                rotateFaceClockwise(cube[3]);
                char temp[3];
                for (int i = 0; i < 3; ++i) temp[i] = cube[0][2 - i][2];
                for (int i = 0; i < 3; ++i) cube[0][2 - i][2] = cube[2][2 - i][2];
                for (int i = 0; i < 3; ++i) cube[2][2 - i][2] = cube[5][2 - i][2];
                for (int i = 0; i < 3; ++i) cube[5][2 - i][2] = cube[4][i][0];
                for (int i = 0; i < 3; ++i) cube[4][i][0] = temp[i];
                break;
            }
            case FACE::BACK: {
                rotateFaceClockwise(cube[4]);
                char temp[3];
                for (int i = 0; i < 3; ++i) temp[i] = cube[0][0][2 - i];
                for (int i = 0; i < 3; ++i) cube[0][0][2 - i] = cube[3][2 - i][2];
                for (int i = 0; i < 3; ++i) cube[3][2 - i][2] = cube[5][2][i];
                for (int i = 0; i < 3; ++i) cube[5][2][i] = cube[1][i][0];
                for (int i = 0; i < 3; ++i) cube[1][i][0] = temp[i];
                break;
            }
            case FACE::DOWN: {
                rotateFaceClockwise(cube[5]);
                char temp[3];
                for (int i = 0; i < 3; ++i) temp[i] = cube[2][2][i];
                for (int i = 0; i < 3; ++i) cube[2][2][i] = cube[1][2][i];
                for (int i = 0; i < 3; ++i) cube[1][2][i] = cube[4][2][i];
                for (int i = 0; i < 3; ++i) cube[4][2][i] = cube[3][2][i];
                for (int i = 0; i < 3; ++i) cube[3][2][i] = temp[i];
                break;
            }
        }
    }
}

RubiksCube& RubiksCube3dArray::L() { applyTurn(FACE::LEFT, 1); return *this; }
RubiksCube& RubiksCube3dArray::LPRIME() { applyTurn(FACE::LEFT, 3); return *this; }
RubiksCube& RubiksCube3dArray::L2() { applyTurn(FACE::LEFT, 2); return *this; }
RubiksCube& RubiksCube3dArray::R() { applyTurn(FACE::RIGHT, 1); return *this; }
RubiksCube& RubiksCube3dArray::RPRIME() { applyTurn(FACE::RIGHT, 3); return *this; }
RubiksCube& RubiksCube3dArray::R2() { applyTurn(FACE::RIGHT, 2); return *this; }
RubiksCube& RubiksCube3dArray::U() { applyTurn(FACE::UP, 1); return *this; }
RubiksCube& RubiksCube3dArray::UPRIME() { applyTurn(FACE::UP, 3); return *this; }
RubiksCube& RubiksCube3dArray::U2() { applyTurn(FACE::UP, 2); return *this; }
RubiksCube& RubiksCube3dArray::D() { applyTurn(FACE::DOWN, 1); return *this; }
RubiksCube& RubiksCube3dArray::DPRIME() { applyTurn(FACE::DOWN, 3); return *this; }
RubiksCube& RubiksCube3dArray::D2() { applyTurn(FACE::DOWN, 2); return *this; }
RubiksCube& RubiksCube3dArray::F() { applyTurn(FACE::FRONT, 1); return *this; }
RubiksCube& RubiksCube3dArray::FPRIME() { applyTurn(FACE::FRONT, 3); return *this; }
RubiksCube& RubiksCube3dArray::F2() { applyTurn(FACE::FRONT, 2); return *this; }
RubiksCube& RubiksCube3dArray::B() { applyTurn(FACE::BACK, 1); return *this; }
RubiksCube& RubiksCube3dArray::BPRIME() { applyTurn(FACE::BACK, 3); return *this; }
RubiksCube& RubiksCube3dArray::B2() { applyTurn(FACE::BACK, 2); return *this; }

bool RubiksCube3dArray::operator==(const RubiksCube3dArray& other) const {
    return std::memcmp(cube, other.cube, sizeof(cube)) == 0;
}

std::size_t RubiksCube3dArray::Hash::operator()(const RubiksCube3dArray& state) const {
    std::size_t hash = 0;
    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                hash = hash * 31 + static_cast<unsigned char>(state.cube[face][row][col]);
            }
        }
    }
    return hash;
}
