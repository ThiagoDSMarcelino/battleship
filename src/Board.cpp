#include "board.h"
#include <iostream>
#include <stdexcept>

Board::Board()
{
    for (auto &row : m_grid)
    {
        row.fill(CellState::Water);
    }
}

bool Board::cells_free(int row, int col, int size, bool horizontal) const
{
    for (int i = 0; i < size; ++i)
    {
        int r = horizontal ? row : row + i;
        int c = horizontal ? col + i : col;
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE)
        {
            return false;
        }
        if (m_grid[r][c] != CellState::Water)
        {
            return false;
        }
    }
    return true;
}

bool Board::place_ship(std::shared_ptr<Ship> ship, int row, int col)
{
    const bool horizontal = ship->orientation() == Orientation::Horizontal;
    if (!cells_free(row, col, ship->size(), horizontal))
    {
        return false;
    }

    for (int i = 0; i < ship->size(); ++i)
    {
        int r = horizontal ? row : row + i;
        int c = horizontal ? col + i : col;
        m_grid[r][c] = CellState::Ship;
        ship->add_cell(r, c);
    }
    m_ships.push_back(std::move(ship));
    return true;
}

bool Board::was_attacked(int row, int col) const
{
    return m_grid[row][col] == CellState::Hit || m_grid[row][col] == CellState::Miss;
}

bool Board::attack(int row, int col)
{
    if (was_attacked(row, col))
    {
        throw std::invalid_argument("Cell already attacked");
    }

    if (m_grid[row][col] == CellState::Ship)
    {
        m_grid[row][col] = CellState::Hit;
        Ship *s = get_ship_at(row, col);
        if (s)
        {
            s->register_hit();
        }
        return true;
    }

    m_grid[row][col] = CellState::Miss;
    return false;
}

bool Board::all_ships_sunk() const
{
    for (const auto &s : m_ships)
    {
        if (!s->is_sunk())
        {
            return false;
        }
    }
    return !m_ships.empty();
}

Ship *Board::get_ship_at(int row, int col) const
{
    for (const auto &s : m_ships)
    {
        if (s->occupies(row, col))
        {
            return s.get();
        }
    }

    return nullptr;
}

char Board::get_char_at(int row, int col, bool hide_ships) const
{
    switch (m_grid[row][col])
    {
    case CellState::Water:
        return '~';
    case CellState::Hit:
        return 'X';
    case CellState::Miss:
        return 'O';
    case CellState::Ship:
    {
        if (hide_ships)
        {
            return '~';
        }

        Ship *s = get_ship_at(row, col);
        return s ? s->cell_char(row, col) : 'S';
    }
    default:
        return '?';
    }
}

void Board::print_header()
{
    std::cout << "   ";
    for (int c = 1; c <= BOARD_SIZE; ++c)
    {
        std::cout << c << (c < 10 ? "  " : " ");
    }
    std::cout << '\n';
}

void Board::print(bool hide_ships) const
{
    print_header();
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        std::cout << static_cast<char>('A' + r) << "  ";
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            std::cout << get_char_at(r, c, hide_ships) << "  ";
        }
        std::cout << '\n';
    }
}
