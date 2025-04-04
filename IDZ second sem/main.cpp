#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class Recusia {
private:

public:
    Recusia(RenderWindow& window, Vector2f a, Vector2f b, Vector2f c, int depth) {
        drawSierpinski(window, a, b, c, depth);
    }


    void drawSierpinski(RenderWindow& window, Vector2f a, Vector2f b, Vector2f c, int depth) {
        if (depth == 0) {
            ConvexShape triangle(3);
            triangle.setPoint(0, a);
            triangle.setPoint(1, b);
            triangle.setPoint(2, c);
            triangle.setFillColor(Color::White);
            window.draw(triangle);
            return;
        }

        Vector2f ab = (a + b) / 2.0f;
        Vector2f bc = (b + c) / 2.0f;
        Vector2f ca = (c + a) / 2.0f;

        drawSierpinski(window, a, ab, ca, depth - 1);
        drawSierpinski(window, ab, b, bc, depth - 1);
        drawSierpinski(window, ca, bc, c, depth - 1);
    }
};

int main() {

    const int WIDTH = 800, HEIGHT = 600;
    RenderWindow window(VideoMode({ WIDTH, HEIGHT }), "");

    View view = window.getDefaultView();
    float zoom_lvl = 1;

    Vector2f p1(WIDTH / 2.0f, 50);
    Vector2f p2(50, HEIGHT - 50);
    Vector2f p3(WIDTH - 50, HEIGHT - 50);

    int depth = 0;
    const int maxDepth = 3;
    Clock clock;


    while (window.isOpen())
    {
        while (const optional event = window.pollEvent()) {
            const auto& e = event.value();
            if (event->is<Event::Closed>())
                window.close();
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    depth++;
                    cout << depth;
                }
        }


        if (Keyboard::isKeyPressed(Keyboard::Key::Up))
        {
            view.zoom(0.9f);
            zoom_lvl *= 0.9f;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Down))
        {
            view.zoom(1.1f);
            zoom_lvl *= 1.1f;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::S))
            view.move({ 0.f, 10.f * zoom_lvl });
        if (Keyboard::isKeyPressed(Keyboard::Key::W))
            view.move({ 0.f, -10.f * zoom_lvl });
        if (Keyboard::isKeyPressed(Keyboard::Key::A))
            view.move({ -10.f * zoom_lvl, 0.f });
        if (Keyboard::isKeyPressed(Keyboard::Key::D))
            view.move({ 10.f * zoom_lvl, 0.f });



        if (clock.getElapsedTime().asSeconds() > 1.0f) {
            if (depth < maxDepth)
                depth++;
            clock.restart();
        }


        window.setView(view);
        window.clear();
        Recusia(window, p1, p2, p3, depth);
        window.display();
    }


    return 0;
}