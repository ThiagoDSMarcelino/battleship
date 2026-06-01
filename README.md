# Battleship

Terminal-based Battleship game written in C++17.

## Requirements

- CMake ≥ 3.16
- Ninja
- A C++17-capable compiler (GCC 9+, Clang 10+)

## Build

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Run

```bash
./build/battleship
```

To skip the interactive placement phase and load a pre-defined layout:

```bash
./build/battleship --layout test_layout.txt
```

### Layout file format

One token per line: coordinate then direction, repeated for each ship in fleet order
(Carrier → Battleship → Cruiser → Submarine → Destroyer).

```text
A1
H
B1
H
C1
H
D1
H
E1
H
```

The path is resolved relative to the working directory where the binary is invoked.

## How to play

### Placement phase

For each ship, enter:

1. Starting coordinate — letter `A`–`J` + number `1`–`10` (e.g. `A1`)
2. Direction — `H` (horizontal) or `V` (vertical)

Ships are drawn with directional characters:

| Symbol | Meaning        |
| ------ | -------------- |
| `<`    | left end       |
| `>`    | right end      |
| `^`    | top end        |
| `v`    | bottom end     |
| `#`    | middle section |
| `~`    | water          |
| `X`    | hit            |
| `O`    | miss           |

### Game phase

Both boards are displayed side by side each turn:

- **Your board** — shows your ships and enemy hits/misses.
- **Enemy board** — shows only your hits (`X`) and misses (`O`); enemy ships are hidden.

Enter a coordinate to attack. The enemy then attacks a random position it hasn't targeted yet.

The game ends when all ships on either side are sunk.

### Fleet

| Ship       | Size |
| ---------- | ---- |
| Carrier    | 5    |
| Battleship | 4    |
| Cruiser    | 3    |
| Submarine  | 3    |
| Destroyer  | 2    |
