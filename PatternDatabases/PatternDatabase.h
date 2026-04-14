#pragma once

#include "../Model/RubiksCube3dArray.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

class PatternDatabase {
public:
    virtual ~PatternDatabase() = default;

    virtual uint8_t getNumMoves(const RubiksCube3dArray& cube) const = 0;
    virtual uint32_t encode(const RubiksCube3dArray& cube) const = 0;

    void generate(const std::string& fileName) {
        if (database.empty()) {
            throw std::runtime_error("Pattern database storage is not initialized");
        }

        std::fill(database.begin(), database.end(), std::numeric_limits<uint8_t>::max());

        std::queue<RubiksCube3dArray> frontier;
        RubiksCube3dArray solvedCube;
        uint32_t solvedIndex = encode(solvedCube);
        database[solvedIndex] = 0;
        frontier.push(solvedCube);

        std::size_t processed = 0;
        while (!frontier.empty()) {
            RubiksCube3dArray current = frontier.front();
            frontier.pop();

            uint8_t currentDepth = database[encode(current)];
            ++processed;

            if (processed % 100000 == 0) {
                std::cout << "Processed states: " << processed << '\n';
            }

            for (int moveIndex = 0; moveIndex < 18; ++moveIndex) {
                RubiksCube3dArray next = current;
                next.move(static_cast<RubiksCube::MOVE>(moveIndex));
                uint32_t nextIndex = encode(next);

                if (database[nextIndex] != std::numeric_limits<uint8_t>::max()) {
                    continue;
                }

                database[nextIndex] = static_cast<uint8_t>(currentDepth + 1);
                frontier.push(next);
            }
        }

        std::ofstream out(fileName, std::ios::binary);
        out.write(reinterpret_cast<const char*>(database.data()),
                  static_cast<std::streamsize>(database.size() * sizeof(uint8_t)));
    }

    void load(const std::string& fileName) {
        std::ifstream in(fileName, std::ios::binary);
        in.read(reinterpret_cast<char*>(database.data()),
                static_cast<std::streamsize>(database.size() * sizeof(uint8_t)));
    }

protected:
    std::vector<uint8_t> database;
};
