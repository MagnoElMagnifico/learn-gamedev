// TODO: project description
#include <SFML/Graphics.hpp>
#include <cmath>

// Config
#define SQR_SZ      70.f
#define WHITE_BG    sf::Color(200, 200, 200)
#define BLACK_BG    sf::Color(10, 10, 10)

int main() {
    sf::RenderWindow window(sf::VideoMode(SQR_SZ * 8, SQR_SZ * 8), "Chess!");
    window.setVerticalSyncEnabled(true);

    sf::RectangleShape square(sf::Vector2f(SQR_SZ, SQR_SZ));
    square.setFillColor(WHITE_BG);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(BLACK_BG);

        // Draw board
        for (int y = 0; y < 8; y++) {
            for (int x = y%2; x < 8; x += 2) {
                square.setPosition(sf::Vector2(SQR_SZ * static_cast<float>(x), SQR_SZ * static_cast<float>(y)));
                window.draw(square);
            }
        }

        window.display();
    }

    return 0;
}
