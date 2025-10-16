# Doublets

A **C++ implementation** of the classic *Word Ladder* puzzle, where you transform one word into another by changing one letter at a time. Each intermediate step must be a valid dictionary word.

## Problem Specification Summary

The **Doublets** puzzle involves generating a chain of words that satisfy the following conditions:

1. Each word in the chain must be a valid dictionary word  
2. Adjacent words differ by exactly one character  
3. All words must have the same length  
4. No word can be repeated in the chain  
5. The goal is to find the **shortest possible path** from the start word to the end word  

For the detailed requirements, see [`spec.pdf`](spec.pdf).

## Makefile Explanation

The [Makefile](doublets/Makefile) builds the Doublets program using a modular compilation approach:

```makefile
doublets: main.o doublets.o
    g++ -g main.o doublets.o -o doublets

main.o: main.cpp doublets.h dictionary.h
    g++ -Wall -g -c main.cpp -o main.o

doublets.o: doublets.cpp doublets.h dictionary.h
    g++ -Wall -g -c doublets.cpp  -o doublets.o

clean:
    rm -f *.o doublets
```

**Build Process:**
- Compiles [`main.cpp`](doublets/main.cpp) and [`doublets.cpp`](doublets/doublets.cpp)  into object files
- Links the object files to create the final executable `doublets`
- Uses `-Wall` for comprehensive warnings and `-g` for debugging symbols
- Automatically rebuilds when headers are modified

**Usage:**
```bash
make          # Build the program
make clean    # Remove object files and executable
./doublets    # Run the program
```

## Input Files

- **`words.txt`**: Dictionary of valid words used for validation
- **`dictionary.h`**: Header file providing dictionary search functionality

## Output Files

- **`chain.txt`**: Generated file containing a valid word chain from the program execution