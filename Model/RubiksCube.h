#pragma once

#include <iostream>
#include <random>
#include <string>
#include <vector>

class RubiksCube {
public:
    enum class FACE { UP = 0, LEFT = 1, FRONT = 2, RIGHT = 3, BACK = 4, DOWN = 5 };
    enum class COLOR { WHITE = 0, GREEN = 1, RED = 2, BLUE = 3, ORANGE = 4, YELLOW = 5 };
    enum class MOVE {
        L, LPRIME, L2,
        R, RPRIME, R2,
        U, UPRIME, U2,
        D, DPRIME, D2,
        F, FPRIME, F2,
        B, BPRIME, B2
    };

    virtual ~RubiksCube() = default;

    virtual COLOR getColor(FACE face, int row, int col) const = 0;
    virtual bool isSolved() const = 0;
    virtual void print() const = 0;

    virtual RubiksCube& L() = 0;
    virtual RubiksCube& LPRIME() = 0;
    virtual RubiksCube& L2() = 0;
    virtual RubiksCube& R() = 0;
    virtual RubiksCube& RPRIME() = 0;
    virtual RubiksCube& R2() = 0;
    virtual RubiksCube& U() = 0;
    virtual RubiksCube& UPRIME() = 0;
    virtual RubiksCube& U2() = 0;
    virtual RubiksCube& D() = 0;
    virtual RubiksCube& DPRIME() = 0;
    virtual RubiksCube& D2() = 0;
    virtual RubiksCube& F() = 0;
    virtual RubiksCube& FPRIME() = 0;
    virtual RubiksCube& F2() = 0;
    virtual RubiksCube& B() = 0;
    virtual RubiksCube& BPRIME() = 0;
    virtual RubiksCube& B2() = 0;

    RubiksCube& move(MOVE ind);
    RubiksCube& shuffle(int moves);
    const std::vector<MOVE>& getShuffleHistory() const;

    static std::string moveToString(MOVE move);
    static MOVE getInverse(MOVE move);

    friend std::ostream& operator<<(std::ostream& out, COLOR color);

protected:
    std::vector<MOVE> shuffleHistory;
};
