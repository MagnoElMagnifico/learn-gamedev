#include "ChessGame.hpp"

namespace Chess {

constexpr float CELL_SZ = 40.0f;
constexpr float SQRT3 = 1.7320508075688772f;
constexpr float CELL_HEIGHT = CELL_SZ * SQRT3;

const sf::Color WHITE_CELL {200, 200, 200};
const sf::Color GREY_CELL  {120, 120, 120};
const sf::Color BLACK_CELL { 30,  30,  30};
const sf::Color COLORS[]   {WHITE_CELL, GREY_CELL, BLACK_CELL};

constexpr char PIECES_PATH[] = "assets/pieces.png";

Game::Game(unsigned int w, unsigned int h) :
    m_window {sf::VideoMode(w, h), "Chess!"},
    m_cell {6}
{
    m_window.setVerticalSyncEnabled(true);

    // A hexagon is built with 6 equilateral triangles.
    // Let a be the side of one triangle, then its height h is:
    //
    //     a^2 = h^2 + (a / 2)^2 => x = sqrt(a^2 - a^2 / 4) = sqrt(a^2 (1 - 1/4))
    //     = a sqrt(3)/2
    //
    m_cell.setPoint(0, sf::Vector2f(0, 0));
    m_cell.setPoint(1, sf::Vector2f(1, 0));

    m_cell.setPoint(3, sf::Vector2f(1, SQRT3));
    m_cell.setPoint(4, sf::Vector2f(0, SQRT3));

    m_cell.setPoint(2, sf::Vector2f(1.5, SQRT3/2));
    m_cell.setPoint(5, sf::Vector2f(-0.5, SQRT3/2));

    m_cell.setScale(CELL_SZ, CELL_SZ);
}

void Game::run() {
    sf::Event event;
    while (m_window.isOpen()) {
        while (m_window.pollEvent(event))
            handleEvent(event);

        // TODO: Only render if event happens
        m_window.clear();
        render();
        m_window.display();
    }
}

void Game::handleEvent(sf::Event event) {
    switch (event.type) {
        case sf::Event::Closed:
            m_window.close();
            break;
        default:
            break;
    }
}

void Game::render() {
    // Render board
    const sf::Vector2f startPos {8.0f * CELL_SZ, 0.0f};
    // Draw center column
    for (int y = 0; y < 11; y++) {
        m_cell.setFillColor(COLORS[y % 3]);
        m_cell.setPosition(startPos + sf::Vector2f(0.0f, static_cast<float>(y)*CELL_HEIGHT));
        m_window.draw(m_cell);
    }

    // Draw the rest using simmetry
    for (int x = 0; x <= 5; x++) {
        for (int y = 0; y < 11 - x; y++) {
            const float fx = static_cast<float>(x);
            const float fy = static_cast<float>(y);
            sf::Vector2f offset {
                fx * 1.5f * CELL_SZ,
                fy * CELL_HEIGHT + fx*CELL_HEIGHT/2.0f
            };

            m_cell.setFillColor(COLORS[(5 - x + y) % 3]);

            m_cell.setPosition(startPos + offset);
            m_window.draw(m_cell);

            offset.x *= -1.0f;
            m_cell.setPosition(startPos + offset);
            m_window.draw(m_cell);
        }
    }
}

} // namespace Chess
