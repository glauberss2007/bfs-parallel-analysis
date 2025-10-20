#include "../include/utils.h"
#include <fstream>
#include <sstream>

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}