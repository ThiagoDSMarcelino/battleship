# Battleship

Terminal-based Battleship game written in C++23.

## Requirements

- [CMake](https://cmake.org/) ≥ 3.16
- [Ninja](https://ninja-build.org/) — the build backend used to compile the project
- A C++23-capable compiler (GCC 12+, Clang 16+)

## Build

**1. Configure** — generate the Ninja build files into the `build/` directory:

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
```

**2. Build** — compile the project:

```bash
ninja -C build
```

## Run

```bash
./build/battleship
```

To load a pre-defined layout instead of placing ships manually, pass it on the command line:

```bash
./build/battleship --layout examples/layout.txt
```

### Layout file format

One ship per line in the format `coordinate direction` (coordinate, a space, then
direction), repeated for each ship in fleet order
(Carrier → Battleship → Cruiser → Submarine → Destroyer).

```text
A1 H
B1 H
C1 H
D1 H
E1 H
```
