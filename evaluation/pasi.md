# Evaluation for micro-machines-1

Project plan evaluation (earlier): 4

## Design

The class structure is documented sufficiently, and use of libraries is
well justified. Use of .hpp and .cpp files is a bit inconsistent:
sometimes headers contain much functional code (e.g. Map.hpp), and
sometimes the whole class definition is in the .cpp file (e.g., main.cpp)

Evaluation: 4

## Working practices

Git is used well and e.g.feature branches have been used.
Quality assurance is well done through Catch2 unit testing.
Work distribution is well documented.

Evaluation: 5

## Implementation

Instead of makefile there are shell scripts to build the software.
That's fine, but they refer specifically to g++7, which is not by default
available e.g. in Ubuntu 20. Although e.g. smart pointers are not use, memory management seems to be quite pedantic, and causes no issues.

Evaluation: 3

# Features

The project group has put much emphasis on features: in addition to in game features, such as networked multiplayer, which can be challenging to do properly sometimes, there are editors for tiles and maps.

Evaluation: 5

Overall: 4.35