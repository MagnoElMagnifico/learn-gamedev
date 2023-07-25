// Chess game class
#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace Chess {

struct Cell {
    int file, rank;
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

