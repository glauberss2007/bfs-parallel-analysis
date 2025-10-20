#include "../../include/graph.h"
#include "../../include/timer.h"
#include <vector>
#include <iostream>
#include <omp.h>
#include <cstdlib>  // Para strtol

std::vector<int> bfs_openmp(const Graph& graph, int source, int num_threads) {
    int n = graph.getNumVertices();
    std::vector<int> distance(n, -1);
    std::vector<int> visited(n, 0);
    
    distance[source] = 0;
    visited[source] = 1;
    
    std::vector<int> frontier = {source};
    const auto& row_ptr = graph.getRowPtr();
    const auto& col_idx = graph.getColIdx();
    
    omp_set_num_threads(num_threads);
    
    while (!frontier.empty()) {
        std::vector<int> next_frontier;
        
        #pragma omp parallel
        {
            std::vector<int> local_frontier;
            
            #pragma omp for schedule(dynamic, 100)
            for (size_t i = 0; i < frontier.size(); i++) {
                int current = frontier[i];
                int start = row_ptr[current];
                int end = row_ptr[current + 1];
                
                for (int j = start; j < end; j++) {
                    int neighbor = col_idx[j];
                    
                    // Use critical section for thread safety
                    #pragma omp critical
                    {
                        if (visited[neighbor] == 0) {
                            visited[neighbor] = 1;
                            distance[neighbor] = distance[current] + 1;
                            local_frontier.push_back(neighbor);
                        }
                    }
                }
            }
            
            #pragma omp critical
            {
                next_frontier.insert(next_frontier.end(), 
                                   local_frontier.begin(), local_frontier.end());
            }
        }
        
        frontier = std::move(next_frontier);
    }
    
    return distance;
}

// Função segura para converter string para int
int safe_stoi(const char* str, int default_value) {
    if (str == nullptr) return default_value;
    try {
        return std::stoi(str);
    } catch (const std::exception& e) {
        return default_value;
    }
}

int main(int argc, char* argv[]) {
    Graph graph;
    std::string filename;
    int source = 0;
    int num_threads = 4;
    
    // Parse arguments safely
    if (argc >= 2) {
        filename = argv[1];
        if (!graph.loadFromFile(filename)) {
            std::cerr << "Failed to load graph from: " << filename << std::endl;
            std::cerr << "Generating random graph instead..." << std::endl;
            graph.generateRandom(1000, 0.01);
        }
    } else {
        std::cout << "No file provided. Generating random graph..." << std::endl;
        graph.generateRandom(1000, 0.01);
    }
    
    // Safe argument parsing
    if (argc >= 3) {
        num_threads = safe_stoi(argv[2], 4);
    }
    if (argc >= 4) {
        source = safe_stoi(argv[3], 0);
    }
    
    std::cout << "Running OpenMP BFS with " << num_threads << " threads" << std::endl;
    graph.printInfo();
    
    Timer timer;
    auto distances = bfs_openmp(graph, source, num_threads);
    double elapsed = timer.elapsed();
    
    std::cout << "OpenMP BFS completed in " << elapsed << " seconds" << std::endl;
    
    int reached = 0;
    int max_distance = 0;
    for (int d : distances) {
        if (d != -1) {
            reached++;
            if (d > max_distance) max_distance = d;
        }
    }
    
    std::cout << "Reached " << reached << "/" << graph.getNumVertices() 
              << " vertices from source " << source << std::endl;
    std::cout << "Maximum distance: " << max_distance << std::endl;
    
    return 0;
}