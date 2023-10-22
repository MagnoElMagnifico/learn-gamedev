#include "ChessGame.hpp"
#include <cmath> // abs, floor
#include <iostream>

#define IS_CELL_INVALID(cell)                                                                 \
    (                                                                                         \
        (cell.file < 0 || cell.file > 10) || cell.rank < 0                                    \
        || (cell.rank > 5 && (cell.file < cell.rank - 5 || cell.file > 10 - (cell.rank - 5))) \
    )                                                                                         \
    // Check the bondaries of the board.
    // The last ranks have a decreasing number of files

namespace Chess {

const sf::Color WHITE_CELL {200, 200, 200};
const sf::Color GREY_CELL  {120, 120, 120};
const sf::Color BLACK_CELL { 30,  30,  30};
const sf::Color COLORS[]   {BLACK_CELL, GREY_CELL, WHITE_CELL};

constexpr float CELL_SZ = 30.0f;
constexpr float SQRT3 = 1.7320508075688772f;
constexpr float CELL_HEIGHT = CELL_SZ * SQRT3;
const sf::FloatRect BOARD {
    100.0f, 100.f,
    17.0f * CELL_SZ,
    11.0f * CELL_HEIGHT
};

constexpr char PIECES_PATH[] = "assets/pieces.png";

HexChess::HexChess(unsigned int w, unsigned int h) :
    m_game {FEN_START},
    m_window {sf::VideoMode(w, h), "Chess!"},
    m_cell {6},
    m_texturePiece {},
    m_piece {m_texturePiece},
    m_textureSize {0}
{
    m_window.setVerticalSyncEnabled(true);

    // ---- Cell setup ----
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

    // ---- Load piece textures ----
    if (!m_texturePiece.loadFromFile(PIECES_PATH))
        throw "Error loading pieces assets";

    m_textureSize = static_cast<int>(m_texturePiece.getSize().x) / 6;

    // The first row matches to the white pieces and the second to the black pieces
    if (static_cast<int>(m_texturePiece.getSize().y) != 2 * m_textureSize)
        throw "Invalid texture size";

    m_piece.scale(0.5, 0.5);
    /* // Resize
    if (static_cast<int>(CELL_SZ) != m_textureSize) {
        // texture / f*cell = 1 => f = cell / texture
        const float scaleFactor = CELL_SZ / static_cast<float>(textureSize);
        m_piece.scale(scaleFactor, scaleFactor);
    } */

    // Debug only
    m_game.printPosition();
}

void HexChess::run() {
    bool needRender = true;

    while (m_window.isOpen()) {
        for (sf::Event event; m_window.pollEvent(event);) {
            handleEvent(event);
            needRender = true;
        }

        if (needRender) {
            m_window.clear();
            render();
            m_window.display();
            needRender = false;
        }
    }
}

void HexChess::handleEvent(sf::Event event) {
    switch (event.type) {
        case sf::Event::Closed:
            m_window.close();
            return;

        case sf::Event::Resized:
            // Dont strech the window contents
            m_window.setView(sf::View(sf::FloatRect(
                0, 0,
                static_cast<float>(event.size.width),
                static_cast<float>(event.size.height)
            )));
            return;

        default:
            return;
    }
}


void HexChess::render() {
    // TODO: support for black's perspective
    auto position = m_game.getPosition();
    for (int file = 0; file < 11; file++) {
        for (int rank = 0; rank < 11; rank++) {
            m_cell.setFillColor(COLORS[((rank > 5)? 10-rank+file : rank+file) % 3]);
            renderCell({rank, file});
            renderPiece({rank, file}, position[static_cast<size_t>(rank*10 + file)]);
        }
    }

    // Highlight cell under cursor
    m_cell.setFillColor({255, 0, 0, 100});
    renderCell(pixelToCell(sf::Mouse::getPosition(m_window)));
}

void HexChess::renderCell(Cell cell) {
    if (IS_CELL_INVALID(cell))
        return;

    const float yOffset = static_cast<float>(std::abs(cell.file - 5)) * CELL_HEIGHT/2.0f;
    m_cell.setPosition({
        static_cast<float>(cell.file) * 1.5f * CELL_SZ + CELL_SZ/2.0f + BOARD.left,
        static_cast<float>(10 - cell.rank) * CELL_HEIGHT - yOffset + BOARD.top
    });
    m_window.draw(m_cell);
}

void HexChess::renderPiece(Cell cell, Piece piece) {
    if (IS_CELL_INVALID(cell) || piece == Piece::None)
        return;

    const int pieceCode = static_cast<int>(piece);
    sf::IntRect textureRect {
        m_textureSize * ((pieceCode > 6)? pieceCode - 8 - 1 : pieceCode - 1),
        (pieceCode > 6)? m_textureSize : 0,
        m_textureSize, m_textureSize
    };
    m_piece.setTextureRect(textureRect);

    const float yOffset = static_cast<float>(std::abs(cell.file - 5)) * CELL_HEIGHT/2.0f;
    m_piece.setPosition({
        static_cast<float>(cell.file) * 1.5f * CELL_SZ + CELL_SZ/2.0f + BOARD.left,
        static_cast<float>(10 - cell.rank) * CELL_HEIGHT - yOffset + BOARD.top
    });

    m_window.draw(m_piece);
}


Cell HexChess::pixelToCell(sf::Vector2i pixel) const {
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
    // fileLeft         fileRight
    //         .-------.
    //        /:       :\    ...
    //       / :       : \   ...
    //      /  :       :  \  ...
    //      \  :       :  /
    //       \ :       : /
    //        \:       :/
    //         .-------.
    //
    const int fileRight = static_cast<int>(normPixel.x / (1.5f*CELL_SZ));
    const int fileLeft  = static_cast<int>((normPixel.x - 0.5f*CELL_SZ) / (1.5f*CELL_SZ));

    // If both estimations have the same result, the given pixel is in the
    // rectangle inside the hexagon.
    // Otherwise, the pixel must the in the side triangles.
    // Note that fileLeft >= fileRight is always true.
    int file = fileRight;
    if (fileRight != fileLeft) {
        // Here, the pixel is between the mentioned rectangles. The coordinates
        // are normalized, and then decide if the pixel is above or below the
        // diagonal.
        // Note that even cells have a decreasing diagonal and odd cells have
        // an increasing diagonal.
        //
        //           x 0 -> 1
        //     .-------.....  0 y
        //    /        :\  :
        //   /fileLeft : \ :     fileRight
        //  /          :..\:  1
        //  \          :  /:
        //   \         : / :
        //    \        :/  :
        //     .-------.....
        //
        const float offsetX = static_cast<float>(fileRight) * 1.5f * CELL_SZ;
        const float offsetY = static_cast<float>(std::abs(fileLeft - 5)) * CELL_HEIGHT/2.0f;
        const float distanceY = (normPixel.y - offsetY) / (CELL_HEIGHT/2.0f);
        const float fracX = (normPixel.x - offsetX) / (CELL_SZ/2.0f);
        const float fracY = distanceY - std::floor(distanceY);

        bool isLeft = (static_cast<int>(distanceY) % 2 == 0)? fracX < fracY : fracX < 1 - fracY;
        file = isLeft? fileLeft : fileRight;
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
