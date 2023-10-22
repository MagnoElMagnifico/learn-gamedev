// TODO: project description
//
// LINKS
// https://en.wikipedia.org/wiki/Hexagonal_chess
// https://youtu.be/bgR3yESAEVE

#include <iostream>
#include "ChessGame.hpp"

int main() {
    try {
        Chess::HexChess game {800, 800};
        game.run();
    } catch(const char* msg) {
        std::cerr << msg << std::endl;
    }

    return 0;
}
