#include "game.h"
#include "utils.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

const std::vector<std::pair<std::string, int>> Game::SHIP_LIST = {
    {"Carrier", 5},
    {"Battleship", 4},
    {"Cruiser", 3},
    {"Submarine", 3},
    {"Destroyer", 2},
};

Game::Game()
    : m_rng(std::random_device{}()),
      m_enemyStrategy(strategy_factory(Difficulty::Medium)) {}

std::expected<void, std::string> Game::run(const std::string &cliLayoutFile)
{
    while (true)
    {
        print_menu();
        std::cout << "  Choose an option: ";

        std::string line;
        if (!std::getline(std::cin, line))
        {
            std::cout << '\n';
            return {}; // end of input -> quit
        }

        std::string choice = trim(line);
        if (choice == "1")
        {
            auto result = start_game(cliLayoutFile);
            if (!result)
            {
                return std::unexpected(result.error());
            }
        }
        else if (choice == "2")
        {
            select_difficulty();
        }
        else if (choice == "3")
        {
            std::cout << "\nThanks for playing!\n";
            return {};
        }
        else
        {
            std::cout << "\n  Invalid option. Choose 1, 2 or 3.\n";
        }
    }
}

void Game::print_menu() const
{
    std::cout << "\n";
    std::cout << "  =============================================\n";
    std::cout << "              B A T T L E S H I P\n";
    std::cout << "  =============================================\n\n";
    std::cout << "    Difficulty: " << m_enemyStrategy->name() << "\n\n";
    std::cout << "    1) Start Game\n";
    std::cout << "    2) Select Difficulty\n";
    std::cout << "    3) Exit\n\n";
}

void Game::select_difficulty()
{
    std::cout << "\n  --- Select Difficulty ---\n\n";
    std::cout << "    1) Easy\n";
    std::cout << "    2) Medium\n";
    std::cout << "    3) Hard\n\n";
    std::cout << "    (All levels currently use random enemy fire.)\n\n";
    std::cout << "    Current: " << m_enemyStrategy->name() << "\n";
    std::cout << "    Choose (1-3, empty to cancel): ";

    std::string line;
    if (!std::getline(std::cin, line))
    {
        return;
    }
    std::string choice = trim(line);

    if (choice.empty())
    {
        return;
    }
    else if (choice == "1")
    {
        m_enemyStrategy = strategy_factory(Difficulty::Easy);
    }
    else if (choice == "2")
    {
        m_enemyStrategy = strategy_factory(Difficulty::Medium);
    }
    else if (choice == "3")
    {
        m_enemyStrategy = strategy_factory(Difficulty::Hard);
    }
    else
    {
        std::cout << "    Invalid choice, keeping " << m_enemyStrategy->name() << ".\n";
        return;
    }
    std::cout << "    Difficulty set to " << m_enemyStrategy->name() << ".\n";
}

std::expected<void, std::string> Game::start_game(const std::string &cliLayoutFile)
{
    reset_boards();

    std::cout << "\n=== BATTLESHIP ===\n\n";

    if (!cliLayoutFile.empty())
    {
        std::ifstream f(cliLayoutFile);
        if (!f)
        {
            return std::unexpected("cannot open layout file \"" + cliLayoutFile + "\"");
        }

        auto loaded = setup_player_ships_from_file(f);
        if (!loaded)
        {
            return std::unexpected(loaded.error());
        }
    }
    else
    {
        setup_player_ships_manual(std::cin);
    }

    setup_enemy_ships();
    std::cout << "\nGame started! Difficulty: " << m_enemyStrategy->name()
              << ". Good luck!\n";

    while (true)
    {
        print_boards();
        if (!player_turn())
        {
            std::cout << "\nInput ended. Returning to menu.\n";
            break;
        }

        if (m_enemyBoard.all_ships_sunk())
        {
            print_boards();
            std::cout << "\nYou sank all enemy ships! VICTORY!\n";
            break;
        }

        enemy_turn();

        if (m_playerBoard.all_ships_sunk())
        {
            print_boards();
            std::cout << "\nAll your ships have been sunk. DEFEAT!\n";
            break;
        }
    }

    // The game loop reads coordinates with operator>>, which leaves the trailing
    // newline in the buffer. Discard it so the next menu getline() starts clean.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return {};
}

void Game::reset_boards()
{
    m_playerBoard = Board{};
    m_enemyBoard = Board{};
}

void Game::setup_player_ships_manual(std::istream &in)
{
    std::cout << "--- Placement Phase ---\n";
    std::cout << "Place your ships on the board (A-J, 1-10).\n";
    std::cout << "Legend: ~ = water  < # > = ship (H)  ^ # v = ship (V)  X = hit  O = miss\n\n";
    m_playerBoard.print(false);

    for (const auto &[name, size] : SHIP_LIST)
    {
        bool placed = false;
        while (!placed)
        {
            std::cout << "\nPlace " << name << " (size " << size << ")\n";
            std::cout << "  Starting coordinate (e.g. A1): ";

            std::string coord;
            if (!(in >> coord))
            {
                return;
            }

            int row, col;
            if (!parse_coord(coord, row, col))
            {
                std::cout << "  Invalid coordinate. Use letter A-J + number 1-10.\n";
                continue;
            }

            std::cout << "  Direction (H=horizontal, V=vertical): ";
            char dir;
            if (!(in >> dir))
            {
                return;
            }
            dir = static_cast<char>(std::toupper(static_cast<unsigned char>(dir)));
            if (dir != 'H' && dir != 'V')
            {
                std::cout << "  Invalid direction. Use H or V.\n";
                continue;
            }

            auto orientation = dir == 'H' ? Orientation::Horizontal : Orientation::Vertical;
            auto ship = std::make_shared<Ship>(name, size, orientation);
            if (!m_playerBoard.place_ship(ship, row, col))
            {
                std::cout << "  Invalid position (out of bounds or overlapping).\n";
                continue;
            }

            placed = true;
            std::cout << '\n';
            m_playerBoard.print(false);
        }
    }

    std::cout << "\nAll ships placed!\n";
}

std::expected<void, std::string> Game::setup_player_ships_from_file(std::istream &in)
{
    for (const auto &[name, size] : SHIP_LIST)
    {
        std::string coord;
        if (!(in >> coord))
        {
            return std::unexpected("layout error: expected a coordinate for " + name);
        }

        int row, col;
        if (!parse_coord(coord, row, col))
        {
            return std::unexpected("layout error: invalid coordinate '" + coord + "' for " + name);
        }

        char dir;
        if (!(in >> dir))
        {
            return std::unexpected("layout error: expected a direction for " + name);
        }
        dir = static_cast<char>(std::toupper(static_cast<unsigned char>(dir)));
        if (dir != 'H' && dir != 'V')
        {
            return std::unexpected(std::string("layout error: invalid direction '") + dir +
                                   "' for " + name + " (use H or V)");
        }

        auto orientation = dir == 'H' ? Orientation::Horizontal : Orientation::Vertical;
        auto ship = std::make_shared<Ship>(name, size, orientation);
        if (!m_playerBoard.place_ship(ship, row, col))
        {
            return std::unexpected("layout error: " + name + " at " + coord +
                                   " is out of bounds or overlaps another ship");
        }
    }

    return {};
}

void Game::setup_enemy_ships()
{
    std::uniform_int_distribution<int> pos(0, BOARD_SIZE - 1);
    std::uniform_int_distribution<int> dir(0, 1);

    for (const auto &[name, size] : SHIP_LIST)
    {
        while (true)
        {
            auto orientation = dir(m_rng) == 0 ? Orientation::Horizontal : Orientation::Vertical;
            auto ship = std::make_shared<Ship>(name, size, orientation);
            if (m_enemyBoard.place_ship(ship, pos(m_rng), pos(m_rng)))
            {
                break;
            }
        }
    }
}

bool Game::player_turn()
{
    while (true)
    {
        std::cout << "\nYour attack (e.g. A1): ";
        std::string input;
        if (!(std::cin >> input))
        {
            return false;
        }

        int row, col;
        if (!parse_coord(input, row, col))
        {
            std::cout << "Invalid coordinate. Use letter A-J + number 1-10.\n";
            continue;
        }

        if (m_enemyBoard.was_attacked(row, col))
        {
            std::cout << "You already attacked that position. Choose another.\n";
            continue;
        }

        bool hit = m_enemyBoard.attack(row, col);
        if (hit)
        {
            Ship *s = m_enemyBoard.get_ship_at(row, col);
            if (s && s->is_sunk())
            {
                std::cout << "Hit and sunk the enemy " << s->name() << "!\n";
            }
            else
            {
                std::cout << "Hit!\n";
            }
        }
        else
        {
            std::cout << "Miss!\n";
        }
        return true;
    }
}

void Game::enemy_turn()
{
    // Delegate target selection to the chosen difficulty's strategy.
    auto [row, col] = m_enemyStrategy->next_attack(m_playerBoard);

    bool hit = m_playerBoard.attack(row, col);
    char letter = static_cast<char>('A' + row);
    std::cout << "\nEnemy attacked " << letter << (col + 1) << ": ";

    if (hit)
    {
        Ship *s = m_playerBoard.get_ship_at(row, col);
        if (s && s->is_sunk())
        {
            std::cout << "Hit and sunk your " << s->name() << "!\n";
        }
        else
        {
            std::cout << "Hit one of your ships!\n";
        }
    }
    else
    {
        std::cout << "Miss!\n";
    }
}

void Game::print_boards() const
{
    auto print_col_header = []()
    {
        std::cout << "   ";
        for (int c = 1; c <= BOARD_SIZE; ++c)
        {
            std::cout << c << (c < 10 ? "  " : " ");
        }
    };

    std::cout << "\n=====================================================================\n";
    std::cout << "  YOUR BOARD                           ENEMY BOARD\n";

    print_col_header();
    std::cout << "    ";
    print_col_header();
    std::cout << '\n';

    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        char letter = static_cast<char>('A' + r);

        std::cout << letter << "  ";
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            std::cout << m_playerBoard.get_char_at(r, c, false) << "  ";
        }

        std::cout << "    " << letter << "  ";
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            std::cout << m_enemyBoard.get_char_at(r, c, true) << "  ";
        }

        std::cout << '\n';
    }
    std::cout << "=====================================================================\n";
}
