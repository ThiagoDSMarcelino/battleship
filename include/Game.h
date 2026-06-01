#pragma once
#include "Board.h"
#include <istream>
#include <memory>
#include <random>
#include <string>
#include <vector>

class Game {
public:
    Game();
    void run(const std::string& layoutFile = "");

private:
    Board m_playerBoard;
    Board m_enemyBoard;

    std::mt19937 m_rng;
    std::vector<std::pair<int,int>> m_enemyTargets;

    void setupPlayerShips(std::istream& in);
    void setupEnemyShips();
    void playerTurn();
    void enemyTurn();
    void printBoards() const;

    static const std::vector<std::pair<std::string,int>> SHIP_LIST;
};
