#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

class Graph {
private:
    std::vector<int> row_ptr;    // CSR row pointers
    std::vector<int> col_idx;    // CSR column indices
    std::vector<int> degrees;    // Vertex degrees
    int num_vertices;
    int num_edges;

public:
    Graph();
    ~Graph();
    
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
    void generateRandom(int n, double probability);
    
    // Getters
    int getNumVertices() const { return num_vertices; }
    int getNumEdges() const { return num_edges; }
    const std::vector<int>& getRowPtr() const { return row_ptr; }
    const std::vector<int>& getColIdx() const { return col_idx; }
    const std::vector<int>& getDegrees() const { return degrees; }
    
    void printInfo() const;
};

#endif