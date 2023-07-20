// TODO: project description
//
// LINKS
// https://en.wikipedia.org/wiki/Hexagonal_chess
// https://youtu.be/bgR3yESAEVE
//
// Project structure:
//  - Game
//      - sf::window
//      - UI
//      - GameState -> Board
//          - ChessPiece[]
//
// Board: hexagon 6 by side
// - option 1
//    - store matrix, each position is a hexagon
//    - more memory
//    - null?
// - option 2
//    - list of pieces, each with a position
//    - less memory
//    - which structure? array?
//
// 9 pawns
#include "ChessGame.hpp"

int main() {
    // // Load pieces texture
    // sf::Texture pieces_texture;
    // assert(pieces_texture.loadFromFile(PIECES_IMG) && "Error loading pieces asset");
    //
    // // There are 6 diferent pieces: pawn, horse/knight, bishop, rook, queen and king
    // // The first row matches to the white pieces and the second to the black pieces
    // unsigned int texture_sz = pieces_texture.getSize().x / 6;
    // assert(pieces_texture.getSize().y == 2*texture_sz && "Wrong asset size for the pieces");
    //
    // sf::Sprite black_pawn(pieces_texture);
    // black_pawn.setTextureRect(sf::IntRect(0, static_cast<int>(texture_sz)*1, static_cast<int>(texture_sz), static_cast<int>(texture_sz)));
    // if (SQR_SZ != texture_sz) {
    //     // texture / f*square = 1 => f = square / texture
    //     float scale_factor = static_cast<float>(SQR_SZ) / static_cast<float>(texture_sz);
    //     black_pawn.scale(scale_factor, scale_factor);
    // }
    // // black_pawn.setColor(sf::Color(100, 100, 100)); // Multiplies all components of the colors
    Chess::Game game {800, 800};
    game.run();

    return 0;
}
