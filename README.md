# bfs-parallel-analysis

Parallel implementations of Breadth-First Search (BFS) algorithm for large-scale complex network analysis, featuring sequential, OpenMP multi-core, and CUDA GPU versions.

## Parallel BFS for Complex Network Analysis

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![OpenMP](https://img.shields.io/badge/OpenMP-4.5-orange.svg)](https://www.openmp.org/)
[![CUDA](https://img.shields.io/badge/CUDA-11.0-green.svg)](https://developer.nvidia.com/cuda-toolkit)

### 📊 Features

- **Sequential BFS** (reference implementation)
- **OpenMP** parallel version for multi-core CPUs
- **CUDA** parallel version for NVIDIA GPUs
- CSR (Compressed Sparse Row) graph representation
- Performance analysis and benchmarking tools
- Support for SNAP dataset format

### 🚀 Quick Start

#### Prerequisites
- C++17 compiler
- OpenMP
- CUDA Toolkit (for GPU version)
- CMake 3.10+

#### Building
```bash
git clone https://github.com/glauberss2007/bfs-parallel-analysis
cd bfs-parallel-analysis

make clean
make sequential
make openmp

./build/bfs_sequential data/sample_graphs/giant_graph.txt

./build/bfs_openmp data/sample_graphs/giant_graph.txt 2
```