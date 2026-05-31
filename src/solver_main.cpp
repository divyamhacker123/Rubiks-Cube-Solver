#include "RubiksCube.h"
#include "PatternDatabase.h"
#include <iostream>

int search(RubiksCube currentCube, int g, int bound, const PatternDatabase& pdb, std::vector<Move>& path) {
    int h = pdb.getNumMoves(currentCube);
    int f = g + h;
    
    if (f > bound) return f;
    if (currentCube.isSolved()) return g;
    
    int min = 1000; 
    
    for (int i = 0; i < 18; ++i) {
        Move nextMove = static_cast<Move>(i);
        
        // Basic optimization: don't twist the same face twice in a row
        if (!path.empty() && (static_cast<int>(path.back()) / 3 == i / 3)) continue;

        RubiksCube nextCube = currentCube;
        nextCube.applyMove(nextMove);
        
        path.push_back(nextMove);
        int t = search(nextCube, g + 1, bound, pdb, path);
        if (t <= bound) return t; // Solved
        if (t < min) min = t;
        path.pop_back();
    }
    return min;
}

int main() {
    std::cout << "Initializing Rubik's Cube Solver..." << std::endl;
    
    PatternDatabase cornerDB("corners.pdb");
    if (!cornerDB.loadFromFile()) {
        std::cerr << "Error: corners.pdb not found. Run DBGenerator first!" << std::endl;
        return 1;
    }
    
    RubiksCube cube;
    std::cout << "Scrambling cube (15 moves)..." << std::endl;
    cube.scramble(15); 
    
    int bound = cornerDB.getNumMoves(cube);
    std::cout << "Initial Heuristic Bound: " << bound << " moves." << std::endl;
    
    std::vector<Move> path;
    while (true) {
        int t = search(cube, 0, bound, cornerDB, path);
        if (t == bound) {
            std::cout << "\nCube solved in " << bound << " moves!" << std::endl;
            break;
        }
        std::cout << "Depth " << bound << " exhausted. Increasing bound to " << t << std::endl;
        bound = t;
    }

    return 0;
}