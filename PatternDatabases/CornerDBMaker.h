#pragma once

#include "CornerPatternDatabase.h"

#include <filesystem>
#include <iostream>
#include <string>

inline void makeCornerPatternDatabase(const std::string& fileName) {
    std::filesystem::path outputPath = fileName.empty()
        ? std::filesystem::path("Databases") / "cornerDepth5V1.txt"
        : std::filesystem::path(fileName);

    if (outputPath.has_parent_path()) {
        std::filesystem::create_directories(outputPath.parent_path());
    }

    std::cout << "Generating corner pattern database at " << outputPath.string() << '\n';
    CornerPatternDatabase database;
    database.generate(outputPath.string());
    std::cout << "Corner pattern database generation complete.\n";
}
