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

## Sample Output

Below is an example of the program execution showing dictionary lookups, valid step checks, and generated word chains across multiple test cases.

```text 
============== Pre-supplied functions ==================

The word ‘ADAPTABLE’ is found in the dictionary.
The word ‘JUMBLEWOCK’ is NOT found in the dictionary.
The word ‘WHEAT’ is found in the dictionary.

====================== Question 1 ======================

From ‘WHEAT’ to ‘CHEAT’ is a valid step.
From ‘WHEAT’ to ‘WHEAD’ is NOT a valid step.
From ‘WHEAT’ to ‘TEA’ is NOT a valid step.
From ‘WHEAT’ to ‘CHEAP’ is NOT a valid step.
From ‘TEA’ to ‘SEA’ is a valid step.

====================== Question 2 ======================

Displaying 7-step chain from ‘WHEAT’ to ‘BREAD’:
WHEAT
cheat
cheap
cheep
creep
creed
breed
BREAD
Output successful!

Writing 4-step chain from ‘TEA’ to ‘HOT’ to file ‘chain.txt’…
Output successful!

====================== Question 3 ======================

The chain from ‘WHEAT’ to ‘BREAD’ is a valid chain.
The chain ‘WHEAT->CHEAP->WHEAT->CHEAP’ is NOT a valid chain.
The chain ‘WHEAT’ is a valid chain.

====================== Question 4 ======================

Found a chain from ‘WET’ to ‘DRY’ with up to 4 steps
DEY
bey
bet
WET

Found a chain from ‘HARD’ to ‘EASY’ with up to 5 steps
EASE
base
bare
bard
HARD

Found a chain from ‘APE’ to ‘MAN’ with up to 6 steps
MAT
eat
eft
aft
apt
APE

Found a chain from ‘BLACK’ to ‘WHITE’ with up to 8 steps
WHINE
chine
chink
chick
click
clack
alack
BLACK

Found a chain from ‘KETTLE’ to ‘HOLDER’ with up to 10 steps
BOLDER
bolter
belter
better
setter
settee
settle
kettle
mettle
KETTLE
```