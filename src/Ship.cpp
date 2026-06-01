#include "Ship.h"

Ship::Ship(std::string name, int size)
    : m_name(std::move(name)), m_size(size), m_hits(0) {}

void Ship::addCell(int row, int col) {
    m_cells.push_back({row, col});
}

bool Ship::occupies(int row, int col) const {
    for (const auto& c : m_cells) {
        if (c.row == row && c.col == col) { return true; }
    }
    return false;
}

void Ship::registerHit() {
    ++m_hits;
}

char Ship::cellChar(int row, int col) const {
    int idx = -1;
    for (int i = 0; i < (int)m_cells.size(); ++i) {
        if (m_cells[i].row == row && m_cells[i].col == col) {
            idx = i;
            break;
        }
    }

    if (idx < 0) { return '?'; }

    bool isFirst = (idx == 0);
    bool isLast  = (idx == (int)m_cells.size() - 1);

    if (m_size == 1) { return '#'; }

    if (m_horizontal) {
        if (isFirst) { return '<'; }
        if (isLast)  { return '>'; }
        return '#';
    } else {
        if (isFirst) { return '^'; }
        if (isLast)  { return 'v'; }
        return '#';
    }
}
