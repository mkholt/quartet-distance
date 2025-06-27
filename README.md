# Phylogenetic Tree Distance Calculator

A streamlined C++ implementation for calculating triplet and quartet distances between phylogenetic trees, based on the master's thesis by Jens Johansen and Morten Kragelund Holt (2013) from Aarhus University.

## Overview

This tool compares phylogenetic trees by computing distances based on triplets (sets of 3 leaves) and quartets (sets of 4 leaves). It implements the optimized algorithms from Brodal et al. using Hierarchical Decomposition Trees (HDT) with novel improvements developed as part of the original thesis research, presented here in a simplified interface for practical use.

## Features

- **Triplet Distance Calculation**: Compare rooted trees based on resolved and unresolved triplets
- **Quartet Distance Calculation**: Compare unrooted trees based on resolved and unresolved quartets (compile-time option)
- **Optimized Algorithm**: Uses Brodal et al. algorithms with HDT for efficient computation
- **Algorithmic Improvements**: Includes thesis improvements like O(min(d₁,d₂)·n·lg n) quartet complexity
- **Arbitrary Degree Trees**: Support for both binary and non-binary trees  
- **Tree Visualization**: Export trees and HDT structures in DOT format for graphical visualization
- **Newick Format Support**: Parse phylogenetic trees in standard Newick format
- **Simplified Interface**: Streamlined command-line interface focused on core functionality

## Building

### Prerequisites

- C++ compiler with support for C++11 or later (g++ recommended)
- Make build system

### Basic Build

```bash
make
```

### Build Options

The Makefile supports the following compilation flags:

- `make QUARTETS=1` - Enable quartet distance calculations (default: triplets only)
- `make NO_N4_128=1` - Use 64-bit integers instead of 128-bit integers
- `make NOEXTRACT=1` - Disable extract-and-contract optimizations
- `make CONTRACT_NUM=<n>` - Set maximum extra size for contractions (default: 100)

### Example Builds

```bash
# Build with quartet support
make QUARTETS=1

# Build with 64-bit integers
make NO_N4_128=1

# Build without extract-and-contract optimizations
make NOEXTRACT=1
```

## Usage

### Basic Syntax

```bash
./trip_bin <command> [filename1] [filename2]
```

### Available Commands

#### Distance Calculations

- `calcTripDist` - Calculate triplet distance between two trees (requires 2 files)
- `calcQuartDist` - Calculate quartet distance between two trees (requires 2 files, QUARTETS=1 build)

```bash
# Calculate triplet distance
./trip_bin calcTripDist tree1.newick tree2.newick

# Calculate quartet distance (requires QUARTETS=1 build)
./quart_bin calcQuartDist tree1.newick tree2.newick
```

#### Tree Visualization

- `printTree` - Export unrooted tree in DOT format for visualization
- `printRootedTree` - Root the tree and export in DOT format  
- `printHDT` - Build HDT of the tree and export in DOT format

```bash
# Export tree for visualization
./trip_bin printTree tree.newick > tree.dot

# Export rooted version
./trip_bin printRootedTree tree.newick > rooted_tree.dot

# Export HDT structure
./trip_bin printHDT tree.newick > hdt.dot
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
- The binary name changes based on build options: `trip_bin` (default) or `quart_bin` (with QUARTETS=1)

## Algorithm Details

### Brodal et al. Algorithm with HDT

This implementation uses the optimized algorithms from Brodal et al. with Hierarchical Decomposition Trees (HDT) and includes novel improvements from the thesis:
- Based on the algorithms from Brodal et al. (2013) with thesis enhancements
- Uses dynamic programming on locally balanced tree decompositions 
- Efficiently handles both resolved and unresolved triplets/quartets on arbitrary degree trees
- **Triplet Distance**: O(n·lg n) time complexity
- **Quartet Distance**: Improved from O(max(d₁,d₂)·n·lg n) to O(min(d₁,d₂)·n·lg n)
- Includes extract-and-contract optimizations for further performance improvements
- Memory optimizations through improved calculation methods (A,E instead of A,B)

### Key Thesis Contributions
- First practical implementation achieving O(n·lg n) triplet distance for arbitrary degree trees
- Novel asymptotic improvement for quartet distance calculation
- Demonstrated feasibility for large-scale trees (1M+ leaves)
- Significant memory usage optimizations

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

- `Soda13Impl.cpp` - Optimized algorithm implementation with thesis improvements
- `HDT.cpp` - Hierarchical Decomposition Tree implementation  
- `HDT*.cpp` - HDT counting and transformation modules
- `NewickParser.cpp` - Newick format parser
- `RootedTree.cpp`, `UnrootedTree.h` - Tree data structures for arbitrary degree trees
- `RootedTreeFactory.cpp`, `HDTFactory.cpp` - Memory-efficient tree construction

## Performance Notes

- Uses the efficient Brodal et al. algorithms with thesis improvements for all tree sizes
- **Scalability**: Successfully handles trees with 1,000,000+ leaves (demonstrated in thesis)
- **Memory efficiency**: HDT representation with optimizations provides practical memory usage
- **Runtime performance**: ~1 minute 45 seconds for quartet distance on 1M leaf binary trees
- Extract-and-contract optimizations are enabled by default for better performance
- 128-bit integer support is enabled by default for handling very large counts

## Examples

```bash
# Compare two phylogenetic trees (triplet distance)
./trip_bin calcTripDist species_tree1.newick species_tree2.newick

# Compare using quartet distance (requires QUARTETS=1 build)
make QUARTETS=1
./quart_bin calcQuartDist species_tree1.newick species_tree2.newick

# Visualize a tree structure
./trip_bin printTree my_tree.newick | dot -Tpng -o tree.png

# Visualize the HDT structure  
./trip_bin printHDT my_tree.newick | dot -Tpng -o hdt.png
```

## Troubleshooting

- **Parsing errors**: Ensure Newick format is valid and properly terminated with semicolon
- **Quartet not available**: Build with `make QUARTETS=1` to enable quartet distance calculations
- **Large integer overflow**: Use `make NO_N4_128=1` if you encounter issues with 128-bit integers
- **Memory issues**: Try building with `make NOEXTRACT=1` to disable optimizations that may use more memory

## References

This implementation is based on and extends the algorithms described in:

- **Brodal, G. S., Fagerberg, R., Mailund, M., Pedersen, C. N. S., and Sand, A.** (2013). "Efficient algorithms for computing the triplet and quartet distance between trees of arbitrary degree." In SODA, pages 1814–1832. SIAM.

The implementation was developed as part of the master's thesis:

- **Johansen, J. and Holt, M. K.** (2013). "Computing Triplet and Quartet Distances." Master's Thesis, Computer Science, Aarhus University. Advisor: Gerth Stølting Brodal.

This simplified branch presents the core thesis algorithms in a streamlined interface while maintaining the key theoretical and practical improvements developed in the original research. The complete thesis document is available as [thesis.pdf](thesis.pdf) in this repository.

