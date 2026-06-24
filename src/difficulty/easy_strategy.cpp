#include "difficulty/easy_strategy.h"

std::string EasyStrategy::name() const
{
    return "Easy";
}

std::pair<int, int> EasyStrategy::next_attack(const Board &board)
{
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
