# 🧊 Optimal Rubik's Cube Solver
> A high-performance, C++ based heuristic search engine capable of finding the optimal (shortest) solution to any scrambled Rubik's Cube. 

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=for-the-badge&logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.10+-success.svg?style=for-the-badge&logo=cmake)
![Algorithm](https://img.shields.io/badge/Algorithm-IDA*-orange.svg?style=for-the-badge)
![Optimization](https://img.shields.io/badge/Optimization-Bitwise-red.svg?style=for-the-badge)

## 📖 Table of Contents
- [Project Overview](#-project-overview)
- [The Science: How It Works](#-the-science-how-it-works)
- [Core Features](#-core-features)
- [Project Architecture](#-project-architecture)
- [Getting Started](#-getting-started)
  - [Prerequisites](#prerequisites)
  - [Build Instructions](#build-instructions)
- [Usage](#-usage)
- [Performance & Benchmarks](#-performance--benchmarks)
- [Future Roadmap](#-future-roadmap)
- [Author](#-author)

---

## 🚀 Project Overview

The standard $3 \times 3 \times 3$ Rubik's Cube has **$4,325,200,327,448,985,600$** ($4.32 \times 10^{19}$) possible configurations. Traditional pathfinding algorithms like standard A* or Breadth-First Search (BFS) exhaust modern memory limits almost instantly when faced with a state space of this magnitude.

This project implements the architecture outlined in Richard E. Korf's seminal 1997 paper, *"Finding Optimal Solutions to Rubik's Cube Using Pattern Databases"*. By translating physical permutations into highly compressed bitwise states and trading memory space for computation time, this solver reliably finds solutions in **18 moves or fewer**.

---

## 🧠 The Science: How It Works

To conquer the mathematical complexity of the cube, this solver utilizes two major theoretical concepts:

### 1. Pattern Databases (The Heuristic)
Instead of relying on weak heuristics like 3D Manhattan distance, this engine pre-computes the exact number of moves required to solve specific subsets of the cube. 
* A reverse BFS is performed from the solved state to map all **88,179,840** possible configurations of the 8 corner pieces.
* The results are compressed down to 4-bits per state (nibbles) and saved as a ~44MB binary lookup table. 
* This provides a mathematically perfect lower-bound heuristic for the search algorithm.

### 2. Iterative-Deepening-A* (IDA*)
IDA* is a depth-first search that looks for solutions within a strictly expanding move limit. 
* The pre-computed Pattern Database tells the algorithm *exactly* how many moves, at minimum, are required to solve the corners from any given state.
* If `(Current Moves + Heuristic) > Current Bound`, the algorithm instantly prunes that branch, saving millions of useless calculations.

---

## ✨ Core Features

* **Extreme Memory Efficiency:** State data is packed into highly compressed 8-bit integers. Lower bits manage physical position `(0-7)`, while upper bits handle 3D orientation `(0-2)`.
* **Perfect Hashing (Lehmer Codes):** Utilizes combinatorial mathematics to map millions of physical piece permutations to unique, sequential integer IDs with zero collisions.
* **Redundancy Pruning:** The move generator intelligently ignores redundant face twists (e.g., turning the same face twice in a row) to slash the search tree's branching factor from 18 down to roughly 13.3.
* **Separation of Concerns:** Powered by CMake, the heavy 44MB database generation is separated from the lightning-fast runtime solver.

---

## 🏗️ Project Architecture

```text
📦 RubiksCubeSolver
 ┣ 📂 include
 ┃ ┣ 📜 RubiksCube.h        # 3D state representation & bitwise math
 ┃ ┗ 📜 PatternDatabase.h   # Lehmer hashing & BFS generation logic
 ┣ 📂 src
 ┃ ┣ 📜 RubiksCube.cpp      # Core permutation engines & face twists
 ┃ ┣ 📜 PatternDatabase.cpp # Heuristic table generation & I/O
 ┃ ┣ 📜 generator_main.cpp  # Executable: Builds the 44MB database
 ┃ ┗ 📜 solver_main.cpp     # Executable: The IDA* search engine
 ┣ 📜 CMakeLists.txt        # Build system configuration
 ┣ 📜 .gitignore            # Keeps the repo clean of build files & .pdb
 ┗ 📜 README.md             # Project documentation



 🛠️ Getting Started
Prerequisites
To compile this engine, your machine must have:

A modern C++ compiler (GCC, Clang, or MSVC) supporting C++17.

CMake (v3.10 or higher).

Build Instructions
This project utilizes an out-of-source build to keep the codebase clean. Ensure you compile in Release Mode; compiler optimizations are critical for the search engine's nodes-per-second performance.


# 1. Clone the repository
git clone [https://github.com/yourusername/RubiksCubeSolver.git](https://github.com/yourusername/RubiksCubeSolver.git)
cd RubiksCubeSolver

# 2. Create the build directory
mkdir build
cd build

# 3. Configure the CMake environment
cmake ..

# 4. Compile the binaries (with optimizations)
cmake --build . --config Release


🕹️ Usage
Because the IDA* algorithm requires the heuristic lookup table to function, you must generate the Pattern Database before running the solver.

Step 1: Generate the Database
Execute the database generator. This will run a massive BFS algorithm and output a corners.pdb file to your directory. Note: You only ever need to run this once!

Windows:
.\Release\DBGenerator.exe

Linux:
./DBGenerator

Step 2: Run the Solver Engine
Once corners.pdb is verified on your disk, launch the solver. The system will scramble a virtual cube and begin the Iterative-Deepening search process.

Windows:

PowerShell
.\Release\Solver.exe
Linux / macOS:

Bash
./Solver
📊 Performance & Benchmarks
Based on the original hardware constraints outlined by Richard E. Korf, this architecture is designed to scale linearly: t≈O(n/m) where t is time, n is state space, and m is memory.

State Generation: Generates and evaluates hundreds of thousands of nodes per second in main memory.

Disk Avoidance: The algorithm strictly forbids reading the heuristic from a disk drive during execution, as disk latency (even on modern NVMe SSDs) bottlenecks the heuristic lookups.

🗺️ Future Roadmap
[ ] Edge Databases: Implement the two additional 20MB lookup tables for the 12 Edge pieces to increase the heuristic bound.

[ ] Heuristic Maximization: Update the solver to take max(Corner_DB, Edge1_DB, Edge2_DB) for aggressive tree pruning.

[ ] Multi-Threading: Dispatch worker threads to explore the primary root branches of the IDA* tree simultaneously.

[ ] Interactive CLI: Allow users to input their own scrambled cube states via console strings.

👨‍💻 Author
Developed by Divyam Gupta and Garv Verma

If you found this repository interesting or educational, feel free to drop a ⭐!