#include "RubiksCubeBitboard.h"

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

RubiksCubeBitboard::RubiksCubeBitboard() : faces{} {
    for (int face = 0; face < 6; ++face) {
        for (int index = 0; index < 8; ++index) {
            setStoredColor(static_cast<FACE>(face), index, static_cast<COLOR>(face));
        }
    }
}

int RubiksCubeBitboard::stickerIndex(int row, int col) {
    if (row == 0) {
        return col;
    }
    if (row == 1 && col == 2) {
        return 3;
    }
    if (row == 2) {
        return 6 - col;
    }
    if (row == 1 && col == 0) {
        return 7;
    }
    return -1;
}

RubiksCube::COLOR RubiksCubeBitboard::getStoredColor(FACE face, int index) const {
    uint32_t value = faces[static_cast<int>(face)];
    uint32_t bits = (value >> (index * 3)) & 0x7U;
    return static_cast<COLOR>(bits);
}

void RubiksCubeBitboard::setStoredColor(FACE face, int index, COLOR color) {
    uint32_t& value = faces[static_cast<int>(face)];
    uint32_t shift = static_cast<uint32_t>(index * 3);
    value &= ~(0x7U << shift);
    value |= (static_cast<uint32_t>(color) & 0x7U) << shift;
}

RubiksCube::COLOR RubiksCubeBitboard::getColor(FACE face, int row, int col) const {
    if (row == 1 && col == 1) {
        return static_cast<COLOR>(face);
    }
    return getStoredColor(face, stickerIndex(row, col));
}

bool RubiksCubeBitboard::isSolved() const {
    for (int face = 0; face < 6; ++face) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (getColor(static_cast<FACE>(face), row, col) != static_cast<COLOR>(face)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void RubiksCubeBitboard::print() const {
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

void RubiksCubeBitboard::applyTurn(FACE face, int quarterTurns) {
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

    std::array<std::array<std::array<COLOR, 3>, 3>, 6> next{};

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
                next[static_cast<int>(destination.face)][destination.row][destination.col] =
                    getColor(static_cast<FACE>(currentFace), row, col);
            }
        }
    }

    for (int currentFace = 0; currentFace < 6; ++currentFace) {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (row == 1 && col == 1) {
                    continue;
                }
                setStoredColor(static_cast<FACE>(currentFace), stickerIndex(row, col),
                               next[currentFace][row][col]);
            }
        }
    }
}

RubiksCube& RubiksCubeBitboard::L() { applyTurn(FACE::LEFT, 1); return *this; }
RubiksCube& RubiksCubeBitboard::LPRIME() { applyTurn(FACE::LEFT, 3); return *this; }
RubiksCube& RubiksCubeBitboard::L2() { applyTurn(FACE::LEFT, 2); return *this; }
RubiksCube& RubiksCubeBitboard::R() { applyTurn(FACE::RIGHT, 1); return *this; }
RubiksCube& RubiksCubeBitboard::RPRIME() { applyTurn(FACE::RIGHT, 3); return *this; }
RubiksCube& RubiksCubeBitboard::R2() { applyTurn(FACE::RIGHT, 2); return *this; }
RubiksCube& RubiksCubeBitboard::U() { applyTurn(FACE::UP, 1); return *this; }
RubiksCube& RubiksCubeBitboard::UPRIME() { applyTurn(FACE::UP, 3); return *this; }
RubiksCube& RubiksCubeBitboard::U2() { applyTurn(FACE::UP, 2); return *this; }
RubiksCube& RubiksCubeBitboard::D() { applyTurn(FACE::DOWN, 1); return *this; }
RubiksCube& RubiksCubeBitboard::DPRIME() { applyTurn(FACE::DOWN, 3); return *this; }
RubiksCube& RubiksCubeBitboard::D2() { applyTurn(FACE::DOWN, 2); return *this; }
RubiksCube& RubiksCubeBitboard::F() { applyTurn(FACE::FRONT, 1); return *this; }
RubiksCube& RubiksCubeBitboard::FPRIME() { applyTurn(FACE::FRONT, 3); return *this; }
RubiksCube& RubiksCubeBitboard::F2() { applyTurn(FACE::FRONT, 2); return *this; }
RubiksCube& RubiksCubeBitboard::B() { applyTurn(FACE::BACK, 1); return *this; }
RubiksCube& RubiksCubeBitboard::BPRIME() { applyTurn(FACE::BACK, 3); return *this; }
RubiksCube& RubiksCubeBitboard::B2() { applyTurn(FACE::BACK, 2); return *this; }

bool RubiksCubeBitboard::operator==(const RubiksCubeBitboard& other) const {
    return faces == other.faces;
}

std::size_t RubiksCubeBitboard::Hash::operator()(const RubiksCubeBitboard& cube) const {
    std::size_t hash = 0;
    for (uint32_t face : cube.faces) {
        hash = hash * 1315423911U + face;
    }
    return hash;
}
