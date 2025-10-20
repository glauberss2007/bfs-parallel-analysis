#include "../../include/graph.h"
#include "../../include/timer.h"
#include <queue>
#include <vector>
#include <iostream>
#include <string>

std::vector<int> bfs_sequential(const Graph& graph, int source) {
    int n = graph.getNumVertices();
    std::vector<int> distance(n, -1);
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    
    distance[source] = 0;
    visited[source] = true;
    q.push(source);
    
    const auto& row_ptr = graph.getRowPtr();
    const auto& col_idx = graph.getColIdx();
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        int start = row_ptr[current];
        int end = row_ptr[current + 1];
        
        for (int i = start; i < end; i++) {
            int neighbor = col_idx[i];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;
                q.push(neighbor);
            }
        }
    }
    
    return distance;
}

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  <filename>          Load graph from file" << std::endl;
    std::cout << "  --generate N P      Generate random graph with N vertices and probability P" << std::endl;
    std::cout << "  --source N          Set source vertex (default: 0)" << std::endl;
}

int main(int argc, char* argv[]) {
    Graph graph;
    int source = 0;
    std::string filename;
    
    // Parse arguments
    if (argc == 1) {
        // No arguments - use default
        std::cout << "No arguments provided. Generating random graph..." << std::endl;
        graph.generateRandom(1000, 0.01);
    } else {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "--generate" && i + 2 < argc) {
                int n = std::stoi(argv[++i]);
                double p = std::stod(argv[++i]);
                graph.generateRandom(n, p);
                filename = "generated";
            } else if (arg == "--source" && i + 1 < argc) {
                source = std::stoi(argv[++i]);
            } else if (arg == "--help") {
                print_usage(argv[0]);
                return 0;
            } else {
                // Assume it's a filename
                filename = arg;
                if (!graph.loadFromFile(filename)) {
                    std::cerr << "Failed to load graph from: " << filename << std::endl;
                    return 1;
                }
            }
        }
    }
    
    graph.printInfo();
    
    Timer timer;
    auto distances = bfs_sequential(graph, source);
    double elapsed = timer.elapsed();
    
    std::cout << "Sequential BFS completed in " << elapsed << " seconds" << std::endl;
    
    int reached = 0;
    for (int d : distances) {
        if (d != -1) reached++;
    }
    std::cout << "Reached " << reached << " vertices from source " << source << std::endl;
    
    return 0;
}