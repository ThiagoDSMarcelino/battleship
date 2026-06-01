#pragma once
#include "Ship.h"
#include <array>
#include <memory>
#include <vector>

constexpr int BOARD_SIZE = 10;

enum class CellState { Water, Ship, Hit, Miss };

class Board {
public:
    Board();

    bool placeShip(std::shared_ptr<Ship> ship, int row, int col, bool horizontal);
    bool attack(int row, int col);  // returns true on hit

    bool allShipsSunk() const;
    bool wasAttacked(int row, int col) const;

    void printOwn() const;
    void printEnemy() const;

    char ownChar(int row, int col) const;
    char enemyChar(int row, int col) const;

    Ship* shipAt(int row, int col) const;

private:
    std::array<std::array<CellState, BOARD_SIZE>, BOARD_SIZE> m_grid;
    std::vector<std::shared_ptr<Ship>> m_ships;

    bool cellsFree(int row, int col, int size, bool horizontal) const;
    static void printHeader();
};
