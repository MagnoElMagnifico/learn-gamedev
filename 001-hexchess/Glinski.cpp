#include "Glinski.hpp"

#include <iostream>

namespace Chess {

GlinskiGame::GlinskiGame(const std::string_view) : m_position{} {
    for (std::size_t i = 0; i < m_position.size(); i++) {
        m_position[i] = Piece::WhitePawn;
    }
}

const GlinskiGame::Position& GlinskiGame::getPosition() const {
    return m_position;
}

// ---- Debug only -----------------------------------------
std::ostream &operator<<(std::ostream &os, Piece const &m) {
    switch (m) {
        case Piece::None: return os << "None";

        case Piece::WhitePawn:   return os << "WhitePawn";
        case Piece::WhiteKnight: return os << "WhiteKnight";
        case Piece::WhiteBishop: return os << "WhiteBishop";
        case Piece::WhiteRook:   return os << "WhiteRook";
        case Piece::WhiteQueen:  return os << "WhiteQueen";
        case Piece::WhiteKing:   return os << "WhiteKing";

        case Piece::BlackPawn:   return os << "BlackPawn";
        case Piece::BlackKnight: return os << "BlackKnight";
        case Piece::BlackBishop: return os << "BlackBishop";
        case Piece::BlackRook:   return os << "BlackRook";
        case Piece::BlackQueen:  return os << "BlackQueen";
        case Piece::BlackKing:   return os << "BlackKing";

        default: return os << "Unsupported piece!";
    }
}

void GlinskiGame::printPosition() const {
    for (std::size_t i = 0; i < m_position.size(); i++) {
        if (i % 11 == 0)
            std::cout << std::endl;
        std::cout << m_position[i] << '\t';
    }
    std::cout << std::endl;
}

} // namespace Chess

