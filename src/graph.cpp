#include "../include/graph.h"
#include "../include/utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>

Graph::Graph() : num_vertices(0), num_edges(0) {}

Graph::~Graph() {}

bool Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }

    // Simple edge list format
    std::vector<std::pair<int, int>> edges;
    int max_vertex = 0;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        int u, v;
        if (iss >> u >> v) {
            edges.push_back({u, v});
            max_vertex = std::max(max_vertex, std::max(u, v));
        }
    }
    
    file.close();

    num_vertices = max_vertex + 1;
    num_edges = edges.size();

    // Build CSR representation
    row_ptr.resize(num_vertices + 1, 0);
    degrees.resize(num_vertices, 0);

    // Count degrees
    for (const auto& edge : edges) {
        degrees[edge.first]++;
    }

    // Build row_ptr
    for (int i = 0; i < num_vertices; i++) {
        row_ptr[i + 1] = row_ptr[i] + degrees[i];
    }

    // Build col_idx
    col_idx.resize(num_edges);
    std::vector<int> temp_degree(num_vertices, 0);
    
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        int pos = row_ptr[u] + temp_degree[u]++;
        col_idx[pos] = v;
    }

    std::cout << "Loaded graph with " << num_vertices << " vertices and " 
              << num_edges << " edges" << std::endl;
    return true;
}

bool Graph::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (int u = 0; u < num_vertices; u++) {
        for (int j = row_ptr[u]; j < row_ptr[u + 1]; j++) {
            file << u << " " << col_idx[j] << "\n";
        }
    }

    file.close();
    return true;
}

void Graph::generateRandom(int n, double probability) {
    num_vertices = n;
    row_ptr.resize(n + 1, 0);
    degrees.resize(n, 0);

    std::vector<std::pair<int, int>> edges;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (dis(gen) < probability) {
                edges.push_back({u, v});
                degrees[u]++;
                degrees[v]++;
            }
        }
    }

    num_edges = edges.size() * 2; // Undirected graph

    // Build row_ptr
    for (int i = 0; i < n; i++) {
        row_ptr[i + 1] = row_ptr[i] + degrees[i];
    }

    // Build col_idx
    col_idx.resize(num_edges);
    std::vector<int> temp_degree(n, 0);
    
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        
        int pos_u = row_ptr[u] + temp_degree[u]++;
        col_idx[pos_u] = v;
        
        int pos_v = row_ptr[v] + temp_degree[v]++;
        col_idx[pos_v] = u;
    }
}

void Graph::printInfo() const {
    std::cout << "Graph Info:" << std::endl;
    std::cout << "  Vertices: " << num_vertices << std::endl;
    std::cout << "  Edges: " << num_edges << std::endl;
    std::cout << "  Density: " << (2.0 * num_edges) / (num_vertices * (num_vertices - 1)) << std::endl;
}