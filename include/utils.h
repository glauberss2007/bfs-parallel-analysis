#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

std::vector<std::string> split(const std::string& str, char delimiter);
bool fileExists(const std::string& filename);

#endif