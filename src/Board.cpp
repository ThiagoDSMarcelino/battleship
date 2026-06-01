#include "Board.h"
#include <iostream>
#include <stdexcept>

Board::Board() {
    for (auto& row : m_grid) {
        row.fill(CellState::Water);
    }
}

bool Board::cellsFree(int row, int col, int size, bool horizontal) const {
    for (int i = 0; i < size; ++i) {
        int r = horizontal ? row : row + i;
        int c = horizontal ? col + i : col;
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) { return false; }
        if (m_grid[r][c] != CellState::Water) { return false; }
    }
    return true;
}

bool Board::placeShip(std::shared_ptr<Ship> ship, int row, int col, bool horizontal) {
    if (!cellsFree(row, col, ship->size(), horizontal)) { return false; }

    ship->setHorizontal(horizontal);
    for (int i = 0; i < ship->size(); ++i) {
        int r = horizontal ? row : row + i;
        int c = horizontal ? col + i : col;
        m_grid[r][c] = CellState::Ship;
        ship->addCell(r, c);
    }
    m_ships.push_back(std::move(ship));
    return true;
}

bool Board::wasAttacked(int row, int col) const {
    return m_grid[row][col] == CellState::Hit || m_grid[row][col] == CellState::Miss;
}

bool Board::attack(int row, int col) {
    if (wasAttacked(row, col)) {
        throw std::invalid_argument("Cell already attacked");
    }

    if (m_grid[row][col] == CellState::Ship) {
        m_grid[row][col] = CellState::Hit;
        Ship* s = shipAt(row, col);
        if (s) { s->registerHit(); }
        return true;
    }

    m_grid[row][col] = CellState::Miss;
    return false;
}

bool Board::allShipsSunk() const {
    for (const auto& s : m_ships) {
        if (!s->isSunk()) { return false; }
    }
    return !m_ships.empty();
}

Ship* Board::shipAt(int row, int col) const {
    for (const auto& s : m_ships) {
        if (s->occupies(row, col)) { return s.get(); }
    }
    return nullptr;
}

char Board::ownChar(int row, int col) const {
    switch (m_grid[row][col]) {
        case CellState::Water: return '~';
        case CellState::Hit:   return 'X';
        case CellState::Miss:  return 'O';
        case CellState::Ship: {
            Ship* s = shipAt(row, col);
            return s ? s->cellChar(row, col) : 'S';
        }
    }
    return '?';
}

char Board::enemyChar(int row, int col) const {
    switch (m_grid[row][col]) {
        case CellState::Hit:  return 'X';
        case CellState::Miss: return 'O';
        default:              return '~';
    }
}

void Board::printHeader() {
    std::cout << "   ";
    for (int c = 1; c <= BOARD_SIZE; ++c) {
        std::cout << c << (c < 10 ? "  " : " ");
    }
    std::cout << '\n';
}

void Board::printOwn() const {
    printHeader();
    for (int r = 0; r < BOARD_SIZE; ++r) {
        std::cout << static_cast<char>('A' + r) << "  ";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            std::cout << ownChar(r, c) << "  ";
        }
        std::cout << '\n';
    }
}

void Board::printEnemy() const {
    printHeader();
    for (int r = 0; r < BOARD_SIZE; ++r) {
        std::cout << static_cast<char>('A' + r) << "  ";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            std::cout << enemyChar(r, c) << "  ";
        }
        std::cout << '\n';
    }
}
