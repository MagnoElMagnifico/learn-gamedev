#pragma once

#include <array>
#include <string>
#include <iostream> // Debug only

namespace Chess {

// White Black CodeWhite CodeBlack PieceType
// P     p     0001      1001      Pawn
// N     n     0010      1010      Knight
// B     b     0011      1011      Bishop
// R     r     0100      1100      Rook
// Q     q     0101      1101      Queen
// K     k     0110      1110      King
// *     *     0000      1000      Non-existent cell
//
//     .----------.
//    |*****b*****| 11
//    |****qbk****| 10
//    |***n b n***|  9
//    |**r     r**|  8
//    |*ppppppppp*|  7
//    |           |  6
//    |     P     |  5
//    |    P P    |  4
//    |   P B P   |  3
//    |  P  B  P  |  2
//    | PRNQBKNRP |  1
//    '-----------'
//     abcdefghikl       // j is not used
/// Code FEN for the canonical initial position
constexpr std::string_view FEN_START =
    "5b5/4qbk4/3n1b1n3/2r5r2/1ppppppppp1/11/5P5/4P1P4/P1B1P3/2P2B2P2/1PRNQBKNRP1 w - 0 1";

// TODO?: Optimize for space, 4 bits per piece
enum class Piece {
    None        = 0,

    // White, 4th bit to 0: 0001 - 0110
    WhitePawn   = 1,
    WhiteKnight = 2,
    WhiteBishop = 3,
    WhiteRook   = 4,
    WhiteQueen  = 5,
    WhiteKing   = 6,

    // Black, 4th bit set: 1001 - 1110
    BlackPawn   = 1 + 8,
    BlackKnight = 2 + 8,
    BlackBishop = 3 + 8,
    BlackRook   = 4 + 8,
    BlackQueen  = 5 + 8,
    BlackKing   = 6 + 8
};

[[maybe_unused]]
static std::ostream &operator<<(std::ostream &os, Piece const &m) {
    switch (m) {
        case Piece::None: return os << ".";

        case Piece::WhitePawn:   return os << "P";
        case Piece::WhiteKnight: return os << "N";
        case Piece::WhiteBishop: return os << "B";
        case Piece::WhiteRook:   return os << "R";
        case Piece::WhiteQueen:  return os << "Q";
        case Piece::WhiteKing:   return os << "K";

        case Piece::BlackPawn:   return os << "p";
        case Piece::BlackKnight: return os << "n";
        case Piece::BlackBishop: return os << "b";
        case Piece::BlackRook:   return os << "r";
        case Piece::BlackQueen:  return os << "q";
        case Piece::BlackKing:   return os << "k";

        default: return os << "Unsupported piece!";
    }
}

/// Glinski's Chess Game Instance
///
/// - Contains all the pieces information
/// - Calculates posible moves
class GlinskiGame {
public:
    using Position = std::array<Piece, 11*11>;

    /// Creates a new game given a FEN cod
    GlinskiGame(const std::string_view);

    /// Used by the game class to render the pieces
    const Position& getPosition() const;

    // Debug only
    void printPosition() const;
private:
    bool m_whiteMoves = true;
    Position m_position;
};

} // namespace Chess

