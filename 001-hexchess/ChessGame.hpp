// Chess game class
#pragma once

#include <array>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace Chess {

struct Cell {
    int file, rank;
};

enum class PieceType {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

struct Piece {
    bool isWhite;
    PieceType pieceType;
};

/// Glinski's Chess Game Instance
///
/// - Contains all the pieces information
/// - Calculates posible moves
class GlinskiGame {
public:
    using Position = std::array<Piece, 11*11>;
    /// Creates a new game given a FEN code
    GlinskiGame(const std::string&);
    /// Used by the game class to render the pieces
    const Position& getPosition() const;
private:
    bool m_whiteMoves = true;
    Position m_position;
};

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
    sf::RenderWindow m_window;
    sf::ConvexShape m_cell;
private:
    void handleEvent(sf::Event);
    void render();
    void renderCell(Cell);
    Cell pixelToCell(sf::Vector2i);
};

} // namespace Chess

