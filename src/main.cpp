#include "Game.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string layoutFile;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--layout" && i + 1 < argc)
            layoutFile = argv[++i];
    }

    Game game;
    game.run(layoutFile);
    return 0;
}
