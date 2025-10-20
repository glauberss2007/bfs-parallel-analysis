CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall
OMP_FLAGS = -fopenmp
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DATA_DIR = data/sample_graphs

# Source files
GRAPH_SOURCES = $(SRC_DIR)/graph.cpp $(SRC_DIR)/timer.cpp $(SRC_DIR)/utils.cpp

# Targets
all: sequential openmp

sequential: $(BUILD_DIR)/bfs_sequential

openmp: $(BUILD_DIR)/bfs_openmp

openmp_v2: $(BUILD_DIR)/bfs_openmp_v2

optimized: $(BUILD_DIR)/bfs_openmp_optimized


$(BUILD_DIR)/bfs_sequential: $(SRC_DIR)/sequential/bfs_sequential.cpp $(GRAPH_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $^ -o $@
	@echo "Built sequential version: $@"

$(BUILD_DIR)/bfs_openmp: $(SRC_DIR)/openmp/bfs_openmp.cpp $(GRAPH_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OMP_FLAGS) -I$(INCLUDE_DIR) $^ -o $@
	@echo "Built OpenMP version: $@"

$(BUILD_DIR)/bfs_openmp_v2: $(SRC_DIR)/openmp/bfs_openmp_v2.cpp $(GRAPH_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OMP_FLAGS) -I$(INCLUDE_DIR) $^ -o $@
	@echo "Built OpenMP v2 version: $@"	

$(BUILD_DIR)/bfs_openmp_optimized: $(SRC_DIR)/openmp/bfs_openmp_optimized.cpp $(GRAPH_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OMP_FLAGS) -I$(INCLUDE_DIR) $^ -o $@
	@echo "Built OpenMP optimized version: $@"

data:
	@mkdir -p $(DATA_DIR)
	@echo "Creating sample graph..."
	@echo "0 1" > $(DATA_DIR)/small_graph.txt
	@echo "0 2" >> $(DATA_DIR)/small_graph.txt
	@echo "1 3" >> $(DATA_DIR)/small_graph.txt
	@echo "2 3" >> $(DATA_DIR)/small_graph.txt
	@echo "3 4" >> $(DATA_DIR)/small_graph.txt
	@echo "4 5" >> $(DATA_DIR)/small_graph.txt
	@echo "Sample graph created at $(DATA_DIR)/small_graph.txt"

test: sequential openmp data
	@echo "Testing sequential version..."
	@cd $(BUILD_DIR) && ./bfs_sequential
	@echo ""
	@echo "Testing OpenMP version..."
	@cd $(BUILD_DIR) && ./bfs_openmp 4

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all sequential openmp data test clean