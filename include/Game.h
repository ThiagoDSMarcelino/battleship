#pragma once
#include "board.h"
#include "enemy_strategy.h"
#include <expected>
#include <istream>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

class Game
{
public:
    Game();
    std::expected<void, std::string> run(const std::string &layoutFile = "");

private:
    Board m_playerBoard;
    Board m_enemyBoard;

    std::mt19937 m_rng;
    std::unique_ptr<EnemyStrategy> m_enemyStrategy;

    void print_menu() const;
    void select_difficulty();

    std::expected<void, std::string> start_game(const std::string &cliLayoutFile);
    void reset_boards();

    void setup_player_ships_manual(std::istream &in);
    std::expected<void, std::string> setup_player_ships_from_file(std::istream &in);
    void setup_enemy_ships();

    bool player_turn();
    void enemy_turn();

    void print_boards() const;

    static const std::vector<std::pair<std::string, int>> SHIP_LIST;
};
