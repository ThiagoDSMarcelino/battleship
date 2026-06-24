#include "enemy_strategy.h"
#include "board.h"
#include "difficulty/easy_strategy.h"
#include "difficulty/medium_strategy.h"
#include "difficulty/hard_strategy.h"
#include <vector>

std::unique_ptr<EnemyStrategy> strategy_factory(Difficulty difficulty)
{
    switch (difficulty)
    {
    case Difficulty::Easy:
        return std::make_unique<EasyStrategy>();
    case Difficulty::Medium:
        return std::make_unique<MediumStrategy>();
    case Difficulty::Hard:
        return std::make_unique<HardStrategy>();
    default:
        return std::make_unique<MediumStrategy>();
    }
}
