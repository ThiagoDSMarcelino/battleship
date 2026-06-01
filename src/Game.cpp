#include "Game.h"
#include "Utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

const std::vector<std::pair<std::string,int>> Game::SHIP_LIST = {
    {"Carrier",    5},
    {"Battleship", 4},
    {"Cruiser",    3},
    {"Submarine",  3},
    {"Destroyer",  2},
};

Game::Game() : m_rng(std::random_device{}()) {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            m_enemyTargets.emplace_back(r, c);
        }
    }
    std::shuffle(m_enemyTargets.begin(), m_enemyTargets.end(), m_rng);
}

void Game::run(const std::string& layoutFile) {
    std::cout << "=== BATTLESHIP ===\n\n";

    if (!layoutFile.empty()) {
        std::ifstream f(layoutFile);
        if (!f) {
            std::cerr << "Error: cannot open layout file '" << layoutFile << "'\n";
            return;
        }
        setupPlayerShips(f);
    } else {
        setupPlayerShips(std::cin);
    }

    setupEnemyShips();
    std::cout << "\nGame started! Good luck!\n";

    while (true) {
        printBoards();
        playerTurn();

        if (m_enemyBoard.allShipsSunk()) {
            printBoards();
            std::cout << "\nYou sank all enemy ships! VICTORY!\n";
            break;
        }

        enemyTurn();

        if (m_playerBoard.allShipsSunk()) {
            printBoards();
            std::cout << "\nAll your ships have been sunk. DEFEAT!\n";
            break;
        }
    }
}

void Game::setupPlayerShips(std::istream& in) {
    std::cout << "--- Placement Phase ---\n";
    std::cout << "Place your ships on the board (A-J, 1-10).\n";
    std::cout << "Legend: ~ = water  < # > = ship (H)  ^ # v = ship (V)  X = hit  O = miss\n\n";
    m_playerBoard.printOwn();

    for (const auto& [name, size] : SHIP_LIST) {
        bool placed = false;
        while (!placed) {
            std::cout << "\nPlace " << name << " (size " << size << ")\n";
            std::cout << "  Starting coordinate (e.g. A1): ";
            std::string coord;
            if (!(in >> coord)) { return; }

            int row, col;
            if (!parseCoord(coord, row, col)) {
                std::cout << "  Invalid coordinate. Use letter A-J + number 1-10.\n";
                continue;
            }

            std::cout << "  Direction (H=horizontal, V=vertical): ";
            char dir;
            if (!(in >> dir)) { return; }
            dir = static_cast<char>(std::toupper(static_cast<unsigned char>(dir)));
            if (dir != 'H' && dir != 'V') {
                std::cout << "  Invalid direction. Use H or V.\n";
                continue;
            }

            auto ship = std::make_shared<Ship>(name, size);
            if (!m_playerBoard.placeShip(ship, row, col, dir == 'H')) {
                std::cout << "  Invalid position (out of bounds or overlapping).\n";
                continue;
            }

            placed = true;
            std::cout << '\n';
            m_playerBoard.printOwn();
        }
    }

    std::cout << "\nAll ships placed!\n";
}

void Game::setupEnemyShips() {
    std::uniform_int_distribution<int> pos(0, BOARD_SIZE - 1);
    std::uniform_int_distribution<int> dir(0, 1);

    for (const auto& [name, size] : SHIP_LIST) {
        while (true) {
            auto ship = std::make_shared<Ship>(name, size);
            if (m_enemyBoard.placeShip(ship, pos(m_rng), pos(m_rng), dir(m_rng) == 0)) {
                break;
            }
        }
    }
}

void Game::playerTurn() {
    while (true) {
        std::cout << "\nYour attack (e.g. A1): ";
        std::string input;
        std::cin >> input;

        int row, col;
        if (!parseCoord(input, row, col)) {
            std::cout << "Invalid coordinate. Use letter A-J + number 1-10.\n";
            continue;
        }

        if (m_enemyBoard.wasAttacked(row, col)) {
            std::cout << "You already attacked that position. Choose another.\n";
            continue;
        }

        bool hit = m_enemyBoard.attack(row, col);
        if (hit) {
            Ship* s = m_enemyBoard.shipAt(row, col);
            if (s && s->isSunk()) {
                std::cout << "Hit and sunk the enemy " << s->name() << "!\n";
            } else {
                std::cout << "Hit!\n";
            }
        } else {
            std::cout << "Miss!\n";
        }
        return;
    }
}

void Game::enemyTurn() {
    auto [row, col] = m_enemyTargets.back();
    m_enemyTargets.pop_back();

    bool hit = m_playerBoard.attack(row, col);
    char letter = static_cast<char>('A' + row);
    std::cout << "\nEnemy attacked " << letter << (col + 1) << ": ";

    if (hit) {
        Ship* s = m_playerBoard.shipAt(row, col);
        if (s && s->isSunk()) {
            std::cout << "Hit and sunk your " << s->name() << "!\n";
        } else {
            std::cout << "Hit one of your ships!\n";
        }
    } else {
        std::cout << "Miss!\n";
    }
}

void Game::printBoards() const {
    auto printColHeader = []() {
        std::cout << "   ";
        for (int c = 1; c <= BOARD_SIZE; ++c) {
            std::cout << c << (c < 10 ? "  " : " ");
        }
    };

    std::cout << "\n=====================================================================\n";
    std::cout << "  YOUR BOARD                           ENEMY BOARD\n";

    printColHeader();
    std::cout << "    ";
    printColHeader();
    std::cout << '\n';

    for (int r = 0; r < BOARD_SIZE; ++r) {
        char letter = static_cast<char>('A' + r);

        std::cout << letter << "  ";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            std::cout << m_playerBoard.ownChar(r, c) << "  ";
        }

        std::cout << "    " << letter << "  ";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            std::cout << m_enemyBoard.enemyChar(r, c) << "  ";
        }

        std::cout << '\n';
    }
    std::cout << "=====================================================================\n";
}
