#pragma once
#include <string>
#include <vector>

struct Cell
{
    int row;
    int col;
};

enum class Orientation
{
    Horizontal,
    Vertical
};

class Ship
{
public:
    Ship(std::string name, int size, Orientation orientation);

    const std::string &name() const { return m_name; }
    int size() const { return m_size; }
    int hits() const { return m_hits; }
    bool is_sunk() const { return m_hits >= m_size; }
    Orientation orientation() const { return m_orientation; }

    void add_cell(int row, int col);
    bool occupies(int row, int col) const;
    void register_hit();

    // Returns the display character for this ship's cell: <#> (horizontal) or ^#v (vertical)
    char cell_char(int row, int col) const;

private:
    std::string m_name;
    int m_size;
    int m_hits;
    Orientation m_orientation;
    std::vector<Cell> m_cells;
};
