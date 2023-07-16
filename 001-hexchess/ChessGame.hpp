// Chess game class
#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

enum ChessPieceType {
    PAWN,
    HORSE,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    PIECE_MAX
};

// class ChessPiece {
// private:
//     ChessPieceType type;
// };

struct ChessConfig {
};

class ChessGame {
public:
    ChessGame();
    void gameLoop();
private:
    // ChessConfig config;
    // sf::RenderWindow window;
    // sf::Texture pieces_texture;
private:
    void handleEvent(sf::Event);
};

