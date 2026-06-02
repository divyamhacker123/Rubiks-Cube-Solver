#include "PatternDatabase.h"
#include <iostream>
#include <fstream>
#include <queue>

PatternDatabase::PatternDatabase(const std::string& filename) : filePath(filename) {
    database.resize(44089920, 0xFF); 
}

// when we will find an scrambled cube then we will compute the corner Index for fast solving the code by looking in the PatternDatabase
uint32_t PatternDatabase::computeCornerIndex(const RubiksCube& cube) const {
    uint32_t orientationIndex = 0;
    // we store the position and orientation in a single 8bits where last 3 bits states positions and remaining bits are orientations
    for (int i = 0; i < 7; ++i) {
        // to avoid the collisions we have multiplied it by 3
        // ex : 1 + 2 = 3 , 2 + 1 = 3 
        // because the corner can be twisted in only three ways (0, 1, 2)
        // orientation wouldn't be bigger than 2
        // 0 -> pointing the correct way
        // 1 -> twisted once(clockwise)
        // 2 -> twisted twice (counter - clockwise)
        orientationIndex = orientationIndex * 3 + (cube.corners[i] >> 3);
    }

    uint32_t positionIndex = 0;
    uint32_t factorial = 1;
    for (int i = 6; i >= 0; --i) {
        int smallerCount = 0;
        for (int j = i + 1; j < 8; ++j) {
            if ((cube.corners[j] & 0x07) < (cube.corners[i] & 0x07)) {
                smallerCount++;
            }
        }
        positionIndex += smallerCount * factorial;
        factorial *= (8 - i);
    }

    return positionIndex * 2187 + orientationIndex;
}

uint8_t PatternDatabase::getNumMoves(const RubiksCube& cube) const {
    return getNumMoves(computeCornerIndex(cube));
}

uint8_t PatternDatabase::getNumMoves(uint32_t index) const {
    uint8_t byteValue = database[index / 2];
    if (index % 2 == 0) return byteValue >> 4;   
    else return byteValue & 0x0F;                
}

void PatternDatabase::setNumMoves(uint32_t index, uint8_t moves) {
    if (index % 2 == 0) {
        database[index / 2] = (database[index / 2] & 0x0F) | (moves << 4);
    } else {
        database[index / 2] = (database[index / 2] & 0xF0) | (moves & 0x0F);
    }
}


// Starting from the fully solved cube
void PatternDatabase::generateCornerDatabase() {
    RubiksCube solvedCube;
    uint32_t solvedIndex = computeCornerIndex(solvedCube);
    setNumMoves(solvedIndex, 0);
    
    std::queue<std::pair<RubiksCube, uint8_t>> q;
    q.push({solvedCube, 0});
    
    uint32_t statesVisited = 1;
    std::cout << "Starting BFS for 88,179,840 states...\n";

    while (!q.empty()) {
        auto [cube, depth] = q.front();
        q.pop();
        
        for (int m = 0; m < 18; ++m) {
            RubiksCube nextCube = cube;
            nextCube.applyMove(static_cast<Move>(m));
            uint32_t nextIndex = computeCornerIndex(nextCube);
            
            if (getNumMoves(nextIndex) == 0x0F) {
                setNumMoves(nextIndex, depth + 1);
                statesVisited++;
                
                if (statesVisited % 5000000 == 0) {
                    std::cout << statesVisited << " states generated..." << std::endl;
                }
                
                if (depth + 1 < 11) {
                    q.push({nextCube, depth + 1});
                }
            }
        }
    }
    std::cout << "Database generation complete. Total states: " << statesVisited << "\n";
}

void PatternDatabase::saveToFile() const {
    std::ofstream out(filePath, std::ios::binary);
    out.write(reinterpret_cast<const char*>(database.data()), database.size());
    out.close();
}

bool PatternDatabase::loadFromFile() {
    std::ifstream in(filePath, std::ios::binary);
    if(in) {
        in.read(reinterpret_cast<char*>(database.data()), database.size());
        in.close();
        return true;
    }
    return false;
}