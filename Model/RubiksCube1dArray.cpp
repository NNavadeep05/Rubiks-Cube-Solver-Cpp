#include "RubiksCube1dArray.h"

#include <cstring>
#include <iostream>

namespace {
struct Sticker {
    int x;
    int y;
    int z;
    int nx;
    int ny;
    int nz;
};

struct FacePosition {
    RubiksCube::FACE face;
    int row;
    int col;
};

Sticker faceToSticker(RubiksCube::FACE face, int row, int col) {
    switch (face) {
        case RubiksCube::FACE::FRONT: return {col - 1, 1 - row, 1, 0, 0, 1};
        case RubiksCube::FACE::BACK: return {1 - col, 1 - row, -1, 0, 0, -1};
        case RubiksCube::FACE::RIGHT: return {1, 1 - row, 1 - col, 1, 0, 0};
        case RubiksCube::FACE::LEFT: return {-1, 1 - row, col - 1, -1, 0, 0};
        case RubiksCube::FACE::UP: return {col - 1, 1, row - 1, 0, 1, 0};
        case RubiksCube::FACE::DOWN: return {col - 1, -1, 1 - row, 0, -1, 0};
    }
    return {};
}

FacePosition stickerToFacePosition(const Sticker& sticker) {
    if (sticker.nz == 1) {
        return {RubiksCube::FACE::FRONT, 1 - sticker.y, sticker.x + 1};
    }
    if (sticker.nz == -1) {
        return {RubiksCube::FACE::BACK, 1 - sticker.y, 1 - sticker.x};
    }
    if (sticker.nx == 1) {
        return {RubiksCube::FACE::RIGHT, 1 - sticker.y, 1 - sticker.z};
    }
    if (sticker.nx == -1) {
        return {RubiksCube::FACE::LEFT, 1 - sticker.y, sticker.z + 1};
    }
    if (sticker.ny == 1) {
        return {RubiksCube::FACE::UP, sticker.z + 1, sticker.x + 1};
    }
    return {RubiksCube::FACE::DOWN, 1 - sticker.z, sticker.x + 1};
}

bool isInLayer(const Sticker& sticker, char axis, int layer) {
    if (axis == 'x') {
        return sticker.x == layer;
    }
    if (axis == 'y') {
        return sticker.y == layer;
    }
    return sticker.z == layer;
}

void rotateQuarter(Sticker& sticker, char axis, int direction) {
    int x = sticker.x;
    int y = sticker.y;
    int z = sticker.z;
    int nx = sticker.nx;
    int ny = sticker.ny;
    int nz = sticker.nz;

    if (axis == 'x') {
        if (direction == 1) {
            sticker.y = -z;
            sticker.z = y;
            sticker.ny = -nz;
            sticker.nz = ny;
        } else {
            sticker.y = z;
            sticker.z = -y;
            sticker.ny = nz;
            sticker.nz = -ny;
        }
        return;
    }

    if (axis == 'y') {
        if (direction == 1) {
            sticker.x = z;
            sticker.z = -x;
            sticker.nx = nz;
            sticker.nz = -nx;
        } else {
            sticker.x = -z;
            sticker.z = x;
            sticker.nx = -nz;
            sticker.nz = nx;
        }
        return;
    }

    if (direction == 1) {
        sticker.x = -y;
        sticker.y = x;
        sticker.nx = -ny;
        sticker.ny = nx;
    } else {
        sticker.x = y;
        sticker.y = -x;
        sticker.nx = ny;
        sticker.ny = -nx;
    }
}
}

RubiksCube1dArray::RubiksCube1dArray() {
    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                cube[index(static_cast<FACE>(face), row, col)] = static_cast<char>(face);
            }
        }
    }
}

int RubiksCube1dArray::index(FACE face, int row, int col) {
    return static_cast<int>(face) * 9 + row * 3 + col;
}

RubiksCube::COLOR RubiksCube1dArray::getColor(FACE face, int row, int col) const {
    return static_cast<COLOR>(cube[index(face, row, col)]);
}

bool RubiksCube1dArray::isSolved() const {
    for (int face = 0; face < 6; ++face) {
        char expected = cube[index(static_cast<FACE>(face), 1, 1)];
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (cube[index(static_cast<FACE>(face), row, col)] != expected) {
                    return false;
                }
            }
        }
    }
    return true;
}

void RubiksCube1dArray::print() const {
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

void RubiksCube1dArray::applyTurn(FACE face, int quarterTurns) {
    quarterTurns = ((quarterTurns % 4) + 4) % 4;
    if (quarterTurns == 0) {
        return;
    }

    char axis = 'x';
    int layer = 0;
    int direction = 1;

    switch (face) {
        case FACE::LEFT: axis = 'x'; layer = -1; direction = 1; break;
        case FACE::RIGHT: axis = 'x'; layer = 1; direction = -1; break;
        case FACE::UP: axis = 'y'; layer = 1; direction = -1; break;
        case FACE::DOWN: axis = 'y'; layer = -1; direction = 1; break;
        case FACE::FRONT: axis = 'z'; layer = 1; direction = -1; break;
        case FACE::BACK: axis = 'z'; layer = -1; direction = 1; break;
    }

    if (quarterTurns == 2) {
        direction = 1;
    }

    char next[54];
    std::memset(next, 0, sizeof(next));

    for (int currentFace = 0; currentFace < 6; ++currentFace) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                Sticker sticker = faceToSticker(static_cast<FACE>(currentFace), row, col);
                if (isInLayer(sticker, axis, layer)) {
                    for (int turn = 0; turn < quarterTurns; ++turn) {
                        rotateQuarter(sticker, axis, direction);
                    }
                }
                FacePosition destination = stickerToFacePosition(sticker);
                next[index(destination.face, destination.row, destination.col)] =
                    cube[index(static_cast<FACE>(currentFace), row, col)];
            }
        }
    }

    std::memcpy(cube, next, sizeof(cube));
}

RubiksCube& RubiksCube1dArray::L() { applyTurn(FACE::LEFT, 1); return *this; }
RubiksCube& RubiksCube1dArray::LPRIME() { applyTurn(FACE::LEFT, 3); return *this; }
RubiksCube& RubiksCube1dArray::L2() { applyTurn(FACE::LEFT, 2); return *this; }
RubiksCube& RubiksCube1dArray::R() { applyTurn(FACE::RIGHT, 1); return *this; }
RubiksCube& RubiksCube1dArray::RPRIME() { applyTurn(FACE::RIGHT, 3); return *this; }
RubiksCube& RubiksCube1dArray::R2() { applyTurn(FACE::RIGHT, 2); return *this; }
RubiksCube& RubiksCube1dArray::U() { applyTurn(FACE::UP, 1); return *this; }
RubiksCube& RubiksCube1dArray::UPRIME() { applyTurn(FACE::UP, 3); return *this; }
RubiksCube& RubiksCube1dArray::U2() { applyTurn(FACE::UP, 2); return *this; }
RubiksCube& RubiksCube1dArray::D() { applyTurn(FACE::DOWN, 1); return *this; }
RubiksCube& RubiksCube1dArray::DPRIME() { applyTurn(FACE::DOWN, 3); return *this; }
RubiksCube& RubiksCube1dArray::D2() { applyTurn(FACE::DOWN, 2); return *this; }
RubiksCube& RubiksCube1dArray::F() { applyTurn(FACE::FRONT, 1); return *this; }
RubiksCube& RubiksCube1dArray::FPRIME() { applyTurn(FACE::FRONT, 3); return *this; }
RubiksCube& RubiksCube1dArray::F2() { applyTurn(FACE::FRONT, 2); return *this; }
RubiksCube& RubiksCube1dArray::B() { applyTurn(FACE::BACK, 1); return *this; }
RubiksCube& RubiksCube1dArray::BPRIME() { applyTurn(FACE::BACK, 3); return *this; }
RubiksCube& RubiksCube1dArray::B2() { applyTurn(FACE::BACK, 2); return *this; }

bool RubiksCube1dArray::operator==(const RubiksCube1dArray& other) const {
    return std::memcmp(cube, other.cube, sizeof(cube)) == 0;
}

std::size_t RubiksCube1dArray::Hash::operator()(const RubiksCube1dArray& state) const {
    std::size_t hash = 0;
    for (char value : state.cube) {
        hash = hash * 31 + static_cast<unsigned char>(value);
    }
    return hash;
}
