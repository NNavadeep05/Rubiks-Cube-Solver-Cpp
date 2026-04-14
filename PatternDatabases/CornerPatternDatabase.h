#pragma once

#include "PatternDatabase.h"

class CornerPatternDatabase : public PatternDatabase {
public:
    CornerPatternDatabase();

    uint8_t getNumMoves(const RubiksCube3dArray& cube) const override;
    uint32_t encode(const RubiksCube3dArray& cube) const override;
};
