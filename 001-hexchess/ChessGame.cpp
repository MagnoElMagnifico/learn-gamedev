#include "ChessGame.hpp"
#include <cmath>    // abs

namespace Chess {

const sf::Color WHITE_CELL {200, 200, 200};
const sf::Color GREY_CELL  {120, 120, 120};
const sf::Color BLACK_CELL { 30,  30,  30};
const sf::Color COLORS[]   {BLACK_CELL, GREY_CELL, WHITE_CELL};

constexpr float CELL_SZ = 30.0f;
const sf::Vector2f START_POS {300.f, 150.f};

constexpr float SQRT3 = 1.7320508075688772f;
constexpr float CELL_HEIGHT = CELL_SZ * SQRT3;
const sf::FloatRect BOARD {
    START_POS.x - 8.0f * CELL_SZ,
    START_POS.y,
    17.0f * CELL_SZ,
    11.0f * CELL_HEIGHT
};

constexpr char PIECES_PATH[] = "assets/pieces.png";

HexChess::HexChess(unsigned int w, unsigned int h) :
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

void HexChess::run() {
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

void HexChess::handleEvent(sf::Event event) {
    switch (event.type) {
        case sf::Event::Closed:
            m_window.close();
            break;
        default:
            break;
    }
}

void HexChess::render() {
    // TODO: support for black's perspective
    for (int x = 0; x < 11; x++)
        for (int y = 0; y < 11; y++) {
            m_cell.setFillColor(COLORS[((x > 5)? 10-x+y : x+y) % 3]);
            renderCell({x, y});
        }

    // Highlight under cursor cell
    m_cell.setFillColor({255, 0, 0});
    renderCell(pixelToCell(sf::Mouse::getPosition(m_window)));

    /*
    sf::RectangleShape rect {{BOARD.width, BOARD.height}};
    rect.setPosition(BOARD.left, BOARD.top);
    rect.setFillColor({200, 0, 0, 100});
    m_window.draw(rect);
    */
}

void HexChess::renderCell(Cell cell) {
    // Only render cells within the board
    // The last ranks have a decreasing number of files
    bool cond = cell.rank > 5 && (cell.file < cell.rank - 5 || cell.file > 10 - (cell.rank - 5));
    if (cell.file < 0 || cell.file > 10 || cell.rank < 0 || cond)
        return;

    const float yOffset = static_cast<float>(std::abs(cell.file - 5)) * CELL_HEIGHT/2.0f;
    m_cell.setPosition({
        static_cast<float>(cell.file) * 1.5f * CELL_SZ + CELL_SZ/2.0f + BOARD.left,
        static_cast<float>(10 - cell.rank) * CELL_HEIGHT - yOffset + BOARD.top
    });
    m_window.draw(m_cell);
}


Cell HexChess::pixelToCell(sf::Vector2i pixel) {
    if (!BOARD.contains(static_cast<float>(pixel.x), static_cast<float>(pixel.y)))
        return {-1, -1};

    // Translate the coordinates to board space
    const sf::Vector2f normPixel {
        static_cast<float>(pixel.x) - BOARD.left,
        static_cast<float>(pixel.y) - BOARD.top
    };

    // To get the file (column), this approximates the hexagon with a rectangle
    // including the leftmost or rightmost vertex.
    //
    //     .-------.....
    //    /:       :\  :
    //   / :       : \ :
    //  /  :       :  \:
    //  \  :       :  /:
    //   \ :       : / :
    //    \:       :/  :
    //     .-------.....
    //
    const int fileLeft  = static_cast<int>(normPixel.x / (1.5f*CELL_SZ));
    const int fileRight = static_cast<int>((normPixel.x - 0.5f*CELL_SZ) / (1.5f*CELL_SZ));

    // If both estimations have the same result, the given pixel is in the
    // rectangle inside the hexagon.
    // Otherwise, the pixel must the in the side triangles.
    const int file = fileLeft;
    if (fileLeft != fileRight) {
        // TODO
    }

    // Since the coordinates are already translated, the middle file can be
    // calculated as if the cells were squares.
    // The other files need an offset; that should be removed later, because the
    // rank is at an angle (otherwise, the calculated rank would be offsetted as
    // well).
    // TODO(bug): the last rank has two cells?
    const float fileOffset = static_cast<float>(std::abs(file - 5));
    const float offset = fileOffset * CELL_HEIGHT/2.0f;
    const int rank = static_cast<int>(((BOARD.height - normPixel.y + offset) / CELL_HEIGHT) - fileOffset);

    return {file, rank};
}

} // namespace Chess
