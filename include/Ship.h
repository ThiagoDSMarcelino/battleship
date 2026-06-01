#pragma once
#include <string>
#include <vector>

struct Cell {
    int row;
    int col;
};

class Ship {
public:
    Ship(std::string name, int size);

    const std::string& name() const { return m_name; }
    int size() const { return m_size; }
    int hits() const { return m_hits; }
    bool isSunk() const { return m_hits >= m_size; }

    void addCell(int row, int col);
    void setHorizontal(bool h) { m_horizontal = h; }
    bool occupies(int row, int col) const;
    void registerHit();

    // Returns the display character for this ship's cell: <#> (horizontal) or ^#v (vertical)
    char cellChar(int row, int col) const;

private:
    std::string m_name;
    int m_size;
    int m_hits;
    bool m_horizontal = true;
    std::vector<Cell> m_cells;
};
