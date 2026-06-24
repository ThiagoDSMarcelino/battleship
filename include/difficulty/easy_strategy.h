#pragma once
#include "enemy_strategy.h"
#include <random>
#include <string>
#include <utility>

class EasyStrategy : public EnemyStrategy
{
public:
    std::string name() const override;
    std::pair<int, int> next_attack(const Board &board) override;

private:
    std::mt19937 m_rng{std::random_device{}()};
};
