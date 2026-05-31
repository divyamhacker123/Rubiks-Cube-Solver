#include "PatternDatabase.h"
#include <iostream>

int main() {
    std::cout << "--- Pattern Database Generator ---" << std::endl;
    PatternDatabase cornerDB("corners.pdb");
    
    cornerDB.generateCornerDatabase();
    
    std::cout << "Writing to disk as corners.pdb..." << std::endl;
    cornerDB.saveToFile();
    
    std::cout << "Done! You can now run the solver executable." << std::endl;
    return 0;
}