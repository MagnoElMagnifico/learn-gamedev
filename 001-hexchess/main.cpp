// TODO: project description
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cassert>

// Config
#define SQR_SZ        80
#define WHITE_BG      sf::Color(200, 200, 200)
#define BLACK_BG      sf::Color(30, 30, 30)
#define HIGHLIGHT_BG  sf::Color(200, 10, 10)
#define PIECES_IMG    "assets/pieces.png"

int main() {
    // Load pieces texture
    sf::Texture pieces_texture;
    assert(pieces_texture.loadFromFile(PIECES_IMG) && "Error loading pieces asset");

    // There are 6 diferent pieces: pawn, horse/knight, bishop, rook, queen and king
    // The first row matches to the white pieces and the second to the black pieces
    unsigned int texture_sz = pieces_texture.getSize().x / 6;
    assert(pieces_texture.getSize().y == 2*texture_sz && "Wrong asset size for the pieces");

    sf::Sprite black_pawn(pieces_texture);
    black_pawn.setTextureRect(sf::IntRect(0, static_cast<int>(texture_sz)*1, static_cast<int>(texture_sz), static_cast<int>(texture_sz)));
    if (SQR_SZ != texture_sz) {
        // texture / f*square = 1 => f = square / texture
        float scale_factor = static_cast<float>(SQR_SZ) / static_cast<float>(texture_sz);
        black_pawn.scale(scale_factor, scale_factor);
    }
    // black_pawn.setColor(sf::Color(100, 100, 100)); // Multiplies all components of the colors

    sf::RenderWindow window(sf::VideoMode(SQR_SZ * 8, SQR_SZ * 8), "Chess!");
    window.setVerticalSyncEnabled(true);

    sf::RectangleShape square(sf::Vector2f(SQR_SZ, SQR_SZ));
    square.setFillColor(WHITE_BG);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonPressed: {
                    sf::Vector2 sqr = sf::Vector2(event.mouseButton.x, event.mouseButton.y) / static_cast<int>(SQR_SZ);
                    std::cout << '(' << sqr.x << ", " << 7-sqr.y << ')' << std::endl;

                } break;

                default:
                    break;
            }
        }

        window.clear(BLACK_BG);

        // Draw board
        for (int y = 0; y < 8; y++) {
            for (int x = y%2; x < 8; x += 2) {
                square.setPosition(sf::Vector2(SQR_SZ * static_cast<float>(x), SQR_SZ * static_cast<float>(y)));
                window.draw(square);
            }
        }

        window.draw(black_pawn);

        window.display();
    }

    return 0;
}
