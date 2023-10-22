// Chess game class
#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Glinski.hpp"

namespace Chess {

struct Cell {
    int file, rank;
};

[[maybe_unused]]
static std::ostream &operator<<(std::ostream &os, Cell const &m) {
    return os << '(' << m.file << ", " << m.rank << ')';
}

/// HexChess Game Class
///
/// - Holds the SFML window and the game instance.
/// - Implements the game loop and event handling
/// - Renders the game: board and pieces
class HexChess {
public:
    HexChess(unsigned int w, unsigned int h);
    void run();
private:
    GlinskiGame m_game;
    sf::RenderWindow m_window;
    sf::ConvexShape m_cell;
    sf::Texture m_texturePiece;
    sf::Sprite m_piece;
    int m_textureSize; // read-only
private:
    void handleEvent(sf::Event);
    void render();
    void renderCell(Cell);
    void renderPiece(Cell, Piece);
    Cell pixelToCell(sf::Vector2i) const;
};

} // namespace Chess

