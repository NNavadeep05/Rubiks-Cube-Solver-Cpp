#include "RubiksCube.h"

#include <array>
#include <stdexcept>

RubiksCube& RubiksCube::move(MOVE ind) {
    switch (ind) {
        case MOVE::L: return L();
        case MOVE::LPRIME: return LPRIME();
        case MOVE::L2: return L2();
        case MOVE::R: return R();
        case MOVE::RPRIME: return RPRIME();
        case MOVE::R2: return R2();
        case MOVE::U: return U();
        case MOVE::UPRIME: return UPRIME();
        case MOVE::U2: return U2();
        case MOVE::D: return D();
        case MOVE::DPRIME: return DPRIME();
        case MOVE::D2: return D2();
        case MOVE::F: return F();
        case MOVE::FPRIME: return FPRIME();
        case MOVE::F2: return F2();
        case MOVE::B: return B();
        case MOVE::BPRIME: return BPRIME();
        case MOVE::B2: return B2();
    }
    throw std::logic_error("Unknown move");
}

RubiksCube& RubiksCube::shuffle(int moves) {
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 17);
    auto faceIndex = [](MOVE move) {
        return static_cast<int>(move) / 3;
    };

    MOVE previousMove = MOVE::L;
    bool hasPrevious = false;

    shuffleHistory.clear();
    shuffleHistory.reserve(moves);

    for (int i = 0; i < moves; ++i) {
        MOVE nextMove;
        do {
            nextMove = static_cast<MOVE>(distribution(generator));
        } while (hasPrevious &&
                 (faceIndex(nextMove) == faceIndex(previousMove) ||
                  getInverse(nextMove) == previousMove));

        move(nextMove);
        shuffleHistory.push_back(nextMove);
        previousMove = nextMove;
        hasPrevious = true;
    }
    return *this;
}

const std::vector<RubiksCube::MOVE>& RubiksCube::getShuffleHistory() const {
    return shuffleHistory;
}

std::string RubiksCube::moveToString(MOVE move) {
    static const std::array<std::string, 18> names = {
        "L", "L'", "L2",
        "R", "R'", "R2",
        "U", "U'", "U2",
        "D", "D'", "D2",
        "F", "F'", "F2",
        "B", "B'", "B2"
    };
    return names[static_cast<int>(move)];
}

RubiksCube::MOVE RubiksCube::getInverse(MOVE move) {
    switch (move) {
        case MOVE::L: return MOVE::LPRIME;
        case MOVE::LPRIME: return MOVE::L;
        case MOVE::L2: return MOVE::L2;
        case MOVE::R: return MOVE::RPRIME;
        case MOVE::RPRIME: return MOVE::R;
        case MOVE::R2: return MOVE::R2;
        case MOVE::U: return MOVE::UPRIME;
        case MOVE::UPRIME: return MOVE::U;
        case MOVE::U2: return MOVE::U2;
        case MOVE::D: return MOVE::DPRIME;
        case MOVE::DPRIME: return MOVE::D;
        case MOVE::D2: return MOVE::D2;
        case MOVE::F: return MOVE::FPRIME;
        case MOVE::FPRIME: return MOVE::F;
        case MOVE::F2: return MOVE::F2;
        case MOVE::B: return MOVE::BPRIME;
        case MOVE::BPRIME: return MOVE::B;
        case MOVE::B2: return MOVE::B2;
    }
    throw std::logic_error("Unknown move");
}

std::ostream& operator<<(std::ostream& out, RubiksCube::COLOR color) {
    switch (color) {
        case RubiksCube::COLOR::WHITE: out << 'W'; break;
        case RubiksCube::COLOR::GREEN: out << 'G'; break;
        case RubiksCube::COLOR::RED: out << 'R'; break;
        case RubiksCube::COLOR::BLUE: out << 'B'; break;
        case RubiksCube::COLOR::ORANGE: out << 'O'; break;
        case RubiksCube::COLOR::YELLOW: out << 'Y'; break;
    }
    return out;
}
