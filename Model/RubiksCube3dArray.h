#pragma once

#include "RubiksCube.h"

#include <cstddef>

class RubiksCube3dArray : public RubiksCube {
public:
    RubiksCube3dArray();

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

    bool operator==(const RubiksCube3dArray& other) const;

    struct Hash {
        std::size_t operator()(const RubiksCube3dArray& cube) const;
    };

private:
    char cube[6][3][3];

    void applyTurn(FACE face, int quarterTurns);
};
