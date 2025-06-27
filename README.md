# Phylogenetic Tree Distance Calculator

A high-performance C++ implementation for calculating triplet and quartet distances between phylogenetic trees, based on the master's thesis by Jens Johansen and Morten Kragelund Holt (2013) from Aarhus University.

## Overview

This tool compares phylogenetic trees by computing distances based on triplets (sets of 3 leaves) and quartets (sets of 4 leaves). It implements both a naive algorithm and optimized algorithms based on the work by Brodal et al. using Hierarchical Decomposition Trees (HDT), with several novel improvements developed as part of this thesis research.

## Research Context

Phylogenetic trees represent evolutionary relationships between species, but different construction methods or datasets can produce different trees for the same set of species. Distance measures help quantify the similarity between such trees.

**Why Triplet and Quartet Distances?**
- **Robust to outliers**: Unlike Robinson-Foulds distance, small changes don't dramatically affect the entire distance
- **Meaningful comparison**: Distinguishes between changes affecting many vs. few relationships
- **General applicability**: Works on both binary and non-binary trees of arbitrary degree

**Previous limitations**:
- Naive algorithms: O(n⁴) for triplets, O(n⁵) for quartets - impractical for large trees
- Existing optimized algorithms: Limited to binary trees or had suboptimal complexity bounds
- Memory constraints: Previous implementations often overflow at small tree sizes (n≈3,000)

## Features

- **Triplet Distance Calculation**: Compare rooted trees based on resolved and unresolved triplets
- **Quartet Distance Calculation**: Compare unrooted trees based on resolved and unresolved quartets  
- **Multiple Algorithms**: 
  - Naive implementation for smaller trees (O(n⁴) for triplets, O(n⁵) for quartets)
  - Optimized algorithm with HDT for larger trees
- **Algorithmic Improvements**: Novel improvements to the original Brodal et al. algorithms:
  - Quartet distance runtime improved from O(max(d₁,d₂)·n·lg n) to O(min(d₁,d₂)·n·lg n)
  - Memory usage reduced from O(max(d₁,d₂)·n·min(max(d₁,d₂),lg n)) to O(min(d₁,d₂)·n·min(d₁,d₂,lg n))
  - Further optimizations by calculating A and E instead of A and B
- **Arbitrary Degree Trees**: Support for both binary and non-binary trees
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
- Time complexity: O(n⁴) for triplets, O(n⁵) for quartets
- Space complexity: O(n³) for triplets, O(n⁴) for quartets

### Optimized Algorithm with Hierarchical Decomposition Trees (HDT)

The optimized algorithm uses Hierarchical Decomposition Trees (HDT) to efficiently count resolved and unresolved triplets/quartets:
- Based on the algorithm from Brodal et al. (2013)
- Uses dynamic programming on locally balanced tree decompositions
- Significantly faster for larger trees
- **Triplet Distance**: O(n·lg n) time, O(n·min(d₁,lg n)) space
- **Quartet Distance (Original)**: O(max(d₁,d₂)·n·lg n) time, O(max(d₁,d₂)·n·min(max(d₁,d₂),lg n)) space
- **Quartet Distance (Improved)**: O(min(d₁,d₂)·n·lg n) time, O(min(d₁,d₂)·n·min(d₁,d₂,lg n)) space

### Key Algorithmic Improvements

This implementation includes several novel improvements over the original Brodal et al. algorithms:

1. **Asymptotic Runtime Improvement**: The quartet distance calculation runtime is improved from O(max(d₁,d₂)·n·lg n) to O(min(d₁,d₂)·n·lg n), which can be very significant when comparing trees with different degrees.

2. **Memory Usage Optimization**: Memory usage is reduced by changing from calculating values A and B to calculating A and E, providing constant factor improvements in both time and space.

3. **Extract & Contract Optimization**: Implements tree contraction techniques that can improve runtime for certain input structures.

### HDT Components

The Hierarchical Decomposition Tree uses four types of components:
- **L**: Leaf nodes in the original tree
- **I**: Internal nodes in the original tree  
- **C**: Connected subsets of nodes in the original tree
- **G**: Sets of subtrees with sibling roots in the original tree

This decomposition ensures the tree remains locally balanced, enabling efficient counting operations.

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

- `main.cpp` - Main program entry point with command-line interface
- `NaiveImpl.cpp` - Naive algorithm implementation for both triplet and quartet distances
- `Soda13Impl.cpp` - Optimized algorithm implementation with HDT and improvements
- `HDT.cpp`, `hdt.h` - Hierarchical Decomposition Tree implementation and utilities
- `NewickParser.cpp`, `newick_parser.h` - Newick format parser for phylogenetic trees
- `RootedTree.cpp`, `rooted_tree.h` - Rooted tree data structures and operations
- `unrooted_tree.h` - Unrooted tree data structures
- `HDTFactory.cpp`, `hdt_factory.h` - Factory for creating HDT structures
- `counting_linked_list.h` - Specialized data structures for efficient counting
- `OutputHandler.cpp`, `output_handler.h` - Tree visualization and output formatting
- `memory_allocator.h` - Custom memory management utilities
- `nvwa/` - Memory debugging and leak detection utilities

## Experimental Validation

The thesis includes comprehensive experimental evaluation demonstrating:

### Performance Benchmarks
- **Large-scale capability**: Successfully processes trees with 1,000,000+ leaves
- **Runtime scaling**: Empirical evidence of O(n·lg n) behavior for triplet distance
- **Memory efficiency**: Practical memory usage even for very large trees
- **Degree impact**: Performance analysis across different tree degrees

### Comparison with Existing Tools
- **Outperforms previous implementations** in both speed and memory usage
- **Handles larger inputs** than existing tools (which typically overflow at n≈3,000)
- **Maintains accuracy** while achieving significant performance improvements

### Test Data Types
- **Fully balanced trees**: Optimal case performance
- **Left-biased trees**: Performance under unbalanced conditions  
- **Random trees**: Real-world typical performance
- **Leaf-moved trees**: Sensitivity analysis

## Performance Notes

- For trees with > 50 leaves, use the `fancy` option for better performance
- **Memory scaling**: For binary trees with 1,000,000 leaves, memory usage is approximately:
  - 11.5GB using A and B calculation method
  - 8.5GB using improved A and E calculation method
- **Runtime performance**: Quartet distance for 1,000,000 leaf binary trees calculated in ~1 minute 45 seconds
- **Degree impact**: Memory usage increases with tree degree (e.g., ~22GB for d=256 with ~630,000 leaves using A,B method, ~15GB using A,E method)
- Enable timing with `SW=1` build flag to monitor performance
- For very large trees, consider the extract-and-contract optimizations
- The implementation can handle trees much larger than previous implementations (which often overflow around n≈3,000)

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

This implementation is based on and extends the algorithms described in:

- **Brodal, G. S., Fagerberg, R., Mailund, M., Pedersen, C. N. S., and Sand, A.** (2013). "Efficient algorithms for computing the triplet and quartet distance between trees of arbitrary degree." In SODA, pages 1814–1832. SIAM.

The implementation was developed as part of the master's thesis:

- **Johansen, J. and Holt, M. K.** (2013). "Computing Triplet and Quartet Distances." Master's Thesis, Computer Science, Aarhus University. Advisor: Gerth Stølting Brodal.

Key contributions of this thesis include:
- First practical implementation of the Brodal et al. algorithms
- Novel asymptotic improvements to quartet distance calculation
- Empirical evaluation showing the algorithms are feasible for large-scale phylogenetic analysis
- Memory usage optimizations and constant factor improvements

The complete thesis document is available as [thesis.pdf](thesis.pdf) in this repository.

