#pragma once
#include "board.h"
#include <memory>
#include <random>
#include <string>
#include <utility>

class EnemyStrategy
{
public:
    virtual ~EnemyStrategy() = default;

    virtual std::string name() const = 0;

    virtual std::pair<int, int> next_attack(const Board &board) = 0;
};

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

std::unique_ptr<EnemyStrategy> strategy_factory(Difficulty difficulty);
