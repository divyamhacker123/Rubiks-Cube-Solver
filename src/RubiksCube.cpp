#include "RubiksCube.h"
#include <iostream>
#include <random>

RubiksCube::RubiksCube() {
    for (uint8_t i = 0; i < 8; ++i) corners[i] = i; 
    for (uint8_t i = 0; i < 12; ++i) edges[i] = i;
}

void RubiksCube::applyMove(Move move) {
    switch (move) {
        case Move::U1: rotateU(); break;
        case Move::U2: rotateU(); rotateU(); break;
        case Move::U3: rotateU(); rotateU(); rotateU(); break;
        
        case Move::D1: rotateD(); break;
        case Move::D2: rotateD(); rotateD(); break;
        case Move::D3: rotateD(); rotateD(); rotateD(); break;
        
        case Move::L1: rotateL(); break;
        case Move::L2: rotateL(); rotateL(); break;
        case Move::L3: rotateL(); rotateL(); rotateL(); break;
        
        case Move::R1: rotateR(); break;
        case Move::R2: rotateR(); rotateR(); break;
        case Move::R3: rotateR(); rotateR(); rotateR(); break;
        
        case Move::F1: rotateF(); break;
        case Move::F2: rotateF(); rotateF(); break;
        case Move::F3: rotateF(); rotateF(); rotateF(); break;
        
        case Move::B1: rotateB(); break;
        case Move::B2: rotateB(); rotateB(); break;
        case Move::B3: rotateB(); rotateB(); rotateB(); break;
    }
}

uint8_t RubiksCube::twist(uint8_t corner, uint8_t amount) {
    uint8_t pos = corner & 0x07;         
    uint8_t ori = (corner >> 3) & 0x03;  
    ori = (ori + amount) % 3;
    return (ori << 3) | pos;
}

void cycle(uint8_t& a, uint8_t& b, uint8_t& c, uint8_t& d) {
    uint8_t temp = d;
    d = c; c = b; b = a; a = temp;
}

void RubiksCube::rotateU() {
    cycle(corners[0], corners[1], corners[2], corners[3]);
    cycle(edges[0], edges[1], edges[2], edges[3]);
}

void RubiksCube::rotateD() {
    cycle(corners[7], corners[6], corners[5], corners[4]);
    cycle(edges[8], edges[9], edges[10], edges[11]);
}

void RubiksCube::rotateF() {
    cycle(corners[3], corners[2], corners[6], corners[7]);
    corners[3] = twist(corners[3], 1);
    corners[2] = twist(corners[2], 2);
    corners[6] = twist(corners[6], 1);
    corners[7] = twist(corners[7], 2);
    cycle(edges[2], edges[5], edges[10], edges[7]);
}

void RubiksCube::rotateB() {
    cycle(corners[1], corners[0], corners[4], corners[5]);
    corners[1] = twist(corners[1], 1);
    corners[0] = twist(corners[0], 2);
    corners[4] = twist(corners[4], 1);
    corners[5] = twist(corners[5], 2);
    cycle(edges[0], edges[4], edges[8], edges[6]);
}

void RubiksCube::rotateL() {
    cycle(corners[0], corners[3], corners[7], corners[4]);
    corners[0] = twist(corners[0], 1);
    corners[3] = twist(corners[3], 2);
    corners[7] = twist(corners[7], 1);
    corners[4] = twist(corners[4], 2);
    cycle(edges[3], edges[7], edges[11], edges[4]);
}

void RubiksCube::rotateR() {
    cycle(corners[2], corners[1], corners[5], corners[6]);
    corners[2] = twist(corners[2], 1);
    corners[1] = twist(corners[1], 2);
    corners[5] = twist(corners[5], 1);
    corners[6] = twist(corners[6], 2);
    cycle(edges[1], edges[6], edges[9], edges[5]);
}

bool RubiksCube::isSolved() const {
    for (uint8_t i = 0; i < 8; ++i) if (corners[i] != i) return false;
    for (uint8_t i = 0; i < 12; ++i) if (edges[i] != i) return false;
    return true;
}

void RubiksCube::scramble(int moves) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 17);
    for (int i = 0; i < moves; ++i) {
        applyMove(static_cast<Move>(dis(gen)));
    }
}