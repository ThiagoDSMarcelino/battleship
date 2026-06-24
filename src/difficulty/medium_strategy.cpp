#include "difficulty/medium_strategy.h"

std::string MediumStrategy::name() const
{
    return "Medium";
}

std::pair<int, int> MediumStrategy::next_attack(const Board &board)
{
    // TODO: Implement that when hits a ship, it will try to attack the surrounding cells to sink it.
    std::vector<std::pair<int, int>> available;
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            if (!board.was_attacked(r, c))
            {
                available.emplace_back(r, c);
            }
        }
    }

    if (available.empty())
    {
        return {0, 0};
    }

    std::uniform_int_distribution<std::size_t> dist(0, available.size() - 1);
    return available[dist(m_rng)];
}
