#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <cstdint>
#include <vector>
#include <string>

enum class Move {
    U1, U2, U3, D1, D2, D3, 
    L1, L2, L3, R1, R2, R3, 
    F1, F2, F3, B1, B2, B3
};

class RubiksCube {
public:
    uint8_t corners[8];
    uint8_t edges[12];

    RubiksCube(); 

    void applyMove(Move move);
    bool isSolved() const;
    void scramble(int moves);
    void print() const;

private:
    uint8_t twist(uint8_t corner, uint8_t amount);
    
    void rotateU();
    void rotateD();
    void rotateL();
    void rotateR();
    void rotateF();
    void rotateB();
};

#endif