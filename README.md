# Phylogenetic Tree Distance Calculator

A high-performance C++ implementation for calculating triplet and quartet distances between phylogenetic trees, with support for both naive and optimized algorithms.

## Overview

This tool compares phylogenetic trees by computing distances based on triplets (sets of 3 leaves) and quartets (sets of 4 leaves). It implements both a naive algorithm and an optimized algorithm based on the Soda13 method with Heavy-light Decomposition Trees (HDT).

## Features

- **Triplet Distance Calculation**: Compare trees based on resolved and unresolved triplets
- **Quartet Distance Calculation**: Compare trees based on resolved and unresolved quartets  
- **Multiple Algorithms**: 
  - Naive implementation for smaller trees
  - Optimized Soda13 algorithm with HDT for larger trees
- **Tree Visualization**: Export trees in DOT format for graphical visualization
- **Tree Generation**: Generate random binary and arbitrary trees for testing
- **Newick Format Support**: Parse phylogenetic trees in standard Newick format
- **Performance Monitoring**: Built-in timing and profiling capabilities

## Building

### Prerequisites

- C++ compiler with support for C++11 or later (g++ recommended)
- Make build system

### Basic Build

```bash
make
```

### Build Options

The Makefile supports various compilation flags:

- `make QUARTETS=1` - Enable quartet distance calculations
- `make DEBUG=1` - Build with debug symbols and memory leak detection
- `make PROF=1` - Build with profiling support
- `make NO_N4_128=1` - Use 64-bit integers instead of 128-bit
- `make SW=1` - Enable stopwatch timing
- `make COUNTER=1` - Enable operation counters

### Example Builds

```bash
# Build with quartet support and debugging
make QUARTETS=1 DEBUG=1

# Build optimized version with timing
make SW=1

# Build with profiling
make PROF=1
```

## Usage

### Basic Syntax

```bash
./trip_bin [options] [filename1] [filename2]
```

### Available Commands

#### Distance Calculations

- `calcTripDist` - Calculate triplet distance between two trees
- `calcQuartDist` - Calculate quartet distance between two trees

```bash
# Calculate triplet distance
./trip_bin calcTripDist tree1.newick tree2.newick

# Calculate quartet distance (requires QUARTETS=1 build)
./trip_bin calcQuartDist tree1.newick tree2.newick

# Use optimized algorithm
./trip_bin fancy calcTripDist tree1.newick tree2.newick
```

#### Tree Analysis and Visualization

- `printTree` - Export tree in DOT format for visualization
- `printRootedTree` - Root the tree and export in DOT format
- `printTriplets` - Print all triplets in a tree
- `printQuartets` - Print all quartets in a tree

```bash
# Export tree for visualization
./trip_bin printTree tree.newick > tree.dot

# Print all triplets (warning: can be very large!)
./trip_bin printTriplets tree.newick
```

#### Tree Generation

- `genTree <n>` - Generate random binary tree with n leaves
- `genTreeArb <n>` - Generate random non-binary tree with n leaves

```bash
# Generate random binary tree with 10 leaves
./trip_bin genTree 10 > random_tree.newick

# Generate random arbitrary tree with 15 leaves  
./trip_bin genTreeArb 15 > random_arb_tree.newick
```

#### HDT Analysis

- `HDT` - Construct HDT and report memory usage
- `HDTPrint` - Construct HDT and export in DOT format

```bash
# Analyze HDT memory usage
./trip_bin HDT tree.newick

# Visualize HDT structure
./trip_bin HDTPrint tree.newick > hdt.dot
```

### Input Format

Trees should be provided in standard Newick format:

```
((A,B),(C,D));
(((A,B),C),D);
```

### Output

- Distance calculations output a single number representing the distance
- Tree exports are in DOT format compatible with Graphviz
- Triplet/quartet listings show all combinations in the specified format

## Algorithm Details

### Naive Algorithm

The naive implementation explicitly enumerates all triplets or quartets and compares their topologies between trees. This approach is:
- Simple and easy to understand
- Suitable for smaller trees (< 50 leaves)
- Time complexity: O(n³) for triplets, O(n⁴) for quartets

### Soda13 Algorithm with HDT

The optimized algorithm uses Heavy-light Decomposition Trees (HDT) to efficiently count resolved and unresolved triplets/quartets:
- Based on the algorithm from Soda et al. (2013)
- Uses dynamic programming on tree decompositions
- Significantly faster for larger trees
- Time complexity: O(n log n) preprocessing + O(n²) counting

## Project Structure

```
├── src/                    # Source code
│   ├── main.cpp           # Main program entry point
│   ├── *Impl.cpp          # Algorithm implementations
│   ├── *.h                # Header files
│   ├── nvwa/              # Memory debugging utilities
│   └── *.vcxproj*         # Visual Studio project files
├── toolTreeGen/           # Tree generation utility
├── Makefile              # Build configuration
└── README.md             # This file
```

### Key Components

- `NaiveImpl.cpp` - Naive algorithm implementation
- `Soda13Impl.cpp` - Optimized Soda13 algorithm
- `HDT.cpp` - Heavy-light Decomposition Tree implementation
- `NewickParser.cpp` - Newick format parser
- `RootedTree.cpp`, `UnrootedTree.h` - Tree data structures

## Performance Notes

- For trees with > 50 leaves, use the `fancy` option for better performance
- Memory usage scales with tree size; HDT can reduce memory overhead
- Enable timing with `SW=1` build flag to monitor performance
- For very large trees, consider the extract-and-contract optimizations

## Examples

```bash
# Compare two phylogenetic trees
./trip_bin fancy calcTripDist species_tree1.newick species_tree2.newick

# Generate test data and compare
./trip_bin genTree 20 > test1.newick
./trip_bin genTree 20 > test2.newick  
./trip_bin fancy calcTripDist test1.newick test2.newick

# Visualize a tree structure
./trip_bin printTree my_tree.newick | dot -Tpng -o tree.png
```

## Troubleshooting

- **Parsing errors**: Ensure Newick format is valid and properly terminated with semicolon
- **Memory issues**: Try building with `DEBUG=1` to detect memory leaks
- **Performance issues**: Use `fancy` algorithm for larger trees
- **Build errors**: Check that all required dependencies are installed

## References

This implementation is based on the algorithm described in:
- Soda et al. (2013) - Efficient algorithms for computing triplet and quartet distances

It was developed as part of the thesis work for:
- Johansen and Holt (2013) - Computing Triplet and Quartet Distances (see [thesis.pdf](thesis.pdf))

