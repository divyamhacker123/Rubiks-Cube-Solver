#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include "RubiksCube.h"
#include <vector>
#include <string>

// Database for stroring the scrambled cube corners state and no of steps to fix the cubies 
class PatternDatabase {
public:
    PatternDatabase(const std::string& filename);

    void generateCornerDatabase();
    uint8_t getNumMoves(const RubiksCube& cube) const;
    void saveToFile() const;
    bool loadFromFile();

private:
    std::string filePath;
    std::vector<uint8_t> database; 
    
    uint32_t computeCornerIndex(const RubiksCube& cube) const;
    uint8_t getNumMoves(uint32_t index) const;
    void setNumMoves(uint32_t index, uint8_t moves);
};

#endif