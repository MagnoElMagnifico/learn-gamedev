#include "Glinski.hpp"

#include <iostream>
#include <cctype> // isdigit
#include <optional>

namespace Chess {

static std::optional<Piece> charToPiece(char piece) {
    int code = 0;

    // Black has the 4th bit set
    if (std::islower(piece))
        code += 8;

    switch (std::tolower(piece)) {
        case 'p': code += 1; break; // Pawn
        case 'n': code += 2; break; // Knight
        case 'b': code += 3; break; // Bishop
        case 'r': code += 4; break; // Rook
        case 'q': code += 5; break; // Queen
        case 'k': code += 6; break; // King
        default: return std::nullopt;
    }

    return static_cast<Piece>(code);
}

GlinskiGame::GlinskiGame(const std::string_view fenCode) : m_position{} {
    // TODO?: validate empty non-existent cells
    unsigned int file = 0;
    unsigned int rank = 10;

    for (std::size_t i = 0; i < fenCode.size(); i++) {
        // New rank
        if (fenCode[i] == '/') {
            rank--;
            file = 0;
            continue;
        }

        // Numbers are empty files to skip
        if (std::isdigit(fenCode[i])) {
            const unsigned int skipFiles = static_cast<unsigned int>(fenCode[i]) - static_cast<unsigned int>('0');

            // Check if the number is multidigit
            if (skipFiles == 1 && fenCode.size() > i+1 && std::isdigit(fenCode[i+1])) {
                i++;
                file += static_cast<unsigned int>(fenCode[i]) - static_cast<unsigned int>('0');
                file += 10u * skipFiles;
            } else {
                file += skipFiles;
            }

            continue;
        }

        // TODO: Extra flags: castling, en-passant, etc
        if (fenCode[i] == ' ')
            break;

        // Read the piece
        m_position[rank*11 + file] = charToPiece(fenCode[i]).value();

        if (file < 11)
            file++;
        else
            throw "Error in FEN code: file exceeds 11";

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

