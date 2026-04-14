#pragma once

#include "RubiksCube.h"

#include <array>
#include <cstddef>
#include <cstdint>

class RubiksCubeBitboard : public RubiksCube {
public:
    RubiksCubeBitboard();

    COLOR getColor(FACE face, int row, int col) const override;
    bool isSolved() const override;
    void print() const override;

    RubiksCube& L() override;
    RubiksCube& LPRIME() override;
    RubiksCube& L2() override;
    RubiksCube& R() override;
    RubiksCube& RPRIME() override;
    RubiksCube& R2() override;
    RubiksCube& U() override;
    RubiksCube& UPRIME() override;
    RubiksCube& U2() override;
    RubiksCube& D() override;
    RubiksCube& DPRIME() override;
    RubiksCube& D2() override;
    RubiksCube& F() override;
    RubiksCube& FPRIME() override;
    RubiksCube& F2() override;
    RubiksCube& B() override;
    RubiksCube& BPRIME() override;
    RubiksCube& B2() override;

    bool operator==(const RubiksCubeBitboard& other) const;

    struct Hash {
        std::size_t operator()(const RubiksCubeBitboard& cube) const;
    };

private:
    std::array<uint32_t, 6> faces;

    static int stickerIndex(int row, int col);
    COLOR getStoredColor(FACE face, int index) const;
    void setStoredColor(FACE face, int index, COLOR color);
    void applyTurn(FACE face, int quarterTurns);
};
