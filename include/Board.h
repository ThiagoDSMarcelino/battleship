#pragma once
#include "ship.h"
#include <array>
#include <memory>
#include <vector>

constexpr int BOARD_SIZE = 10;

enum class CellState
{
    Water,
    Ship,
    Hit,
    Miss
};

class Board
{
public:
    Board();

    bool place_ship(std::shared_ptr<Ship> ship, int row, int col);

    // returns true on hit
    bool attack(int row, int col);

    bool all_ships_sunk() const;

    bool was_attacked(int row, int col) const;

    // hide_ships = true renders un-hit ships as water (opponent's view).
    void print(bool hide_ships) const;

    char get_char_at(int row, int col, bool hide_ships) const;

    Ship *get_ship_at(int row, int col) const;

private:
    std::array<std::array<CellState, BOARD_SIZE>, BOARD_SIZE> m_grid;
    std::vector<std::shared_ptr<Ship>> m_ships;

    bool cells_free(int row, int col, int size, bool horizontal) const;
    static void print_header();
};
