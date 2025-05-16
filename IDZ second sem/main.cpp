#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <windows.h>


#ifndef PI
#define PI 3.14159265358979323846
#endif

using namespace sf;
using namespace std;

const int WIDTH = 800, HEIGHT = 600;
const int maxDepth = 3;

class Obj {
private:
    const int MyWidth, MyHeight;
public:
    Obj(int value1, int value2) : MyWidth(value1), MyHeight(value2) {}
    virtual void drawRec(RenderWindow& window, int depth) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////
class SperTriangl: public Obj {
private:
    Vector2f a;
    Vector2f b;
    Vector2f c;
    void drawRecusive(RenderWindow& window, Vector2f a, Vector2f b, Vector2f c, int depth) {
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

        drawRecusive(window, a, ab, ca, depth - 1);
        drawRecusive(window, ab, b, bc, depth - 1);
        drawRecusive(window, ca, bc, c, depth - 1);
    }
public:
    SperTriangl() : Obj(WIDTH, HEIGHT), a(WIDTH / 2.0f, 50), b(50, HEIGHT - 50), c(WIDTH - 50, HEIGHT - 50) {}
    void drawRec(RenderWindow& window, int depth) override {drawRecusive(window, a, b, c, depth);}
};

/////////////////////////////////////////////////////////////////////////////////////
class KochSnowflake : public Obj {
private:
    Vector2f a, b, c;

    void drawEdge(RenderWindow& window, Vector2f p1, Vector2f p2, int depth) {
        if (depth == 0) {
            sf::VertexArray line(PrimitiveType::Lines, 2);
            line[0].position = p1;
            line[0].color = Color::White;
            line[1].position = p2;
            line[1].color = Color::White;
            window.draw(line);
            return;
        }

        Vector2f one_third = p1 + (p2 - p1) / 3.f;
        Vector2f two_third = p1 + (p2 - p1) * 2.f / 3.f;

        Vector2f dir = two_third - one_third;

        float angle_rad = PI / 3.f;
        float cosA = cos(angle_rad);
        float sinA = sin(angle_rad);

        Vector2f peak{
            one_third.x + cosA * dir.x - sinA * dir.y,
            one_third.y + sinA * dir.x + cosA * dir.y
        };

        drawEdge(window, p1, one_third, depth - 1);
        drawEdge(window, one_third, peak, depth - 1);
        drawEdge(window, peak, two_third, depth - 1);
        drawEdge(window, two_third, p2, depth - 1);
    }


public:
    KochSnowflake() : Obj(WIDTH, HEIGHT) {
        float side = WIDTH * 0.6f;
        float height = side * sqrt(3.f) / 2.f;

        a = Vector2f(WIDTH / 2.f, HEIGHT / 2.f - height / 2.f);
        b = Vector2f(WIDTH / 2.f - side / 2.f, HEIGHT / 2.f + height / 2.f);
        c = Vector2f(WIDTH / 2.f + side / 2.f, HEIGHT / 2.f + height / 2.f);
    }

    void drawRec(RenderWindow& window, int depth) override {
        drawEdge(window, a, b, depth);
        drawEdge(window, b, c, depth);
        drawEdge(window, c, a, depth);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class FractalTree : public Obj {
private:
    void drawBranch(RenderWindow& window, sf::Vector2f start, float angle, float length, int depth) {
        if (depth == 0)
            return;

        Vector2f end = start + Vector2f(length * cos(angle), length * sin(angle));

        VertexArray branch(PrimitiveType::Lines, 2);
        branch[0].position = start;
        branch[0].color = Color::White;
        branch[1].position = end;
        branch[1].color = Color::White;
        window.draw(branch);

        float angleOffset = PI / 6.f;
        float newLength = length * 0.7f;

        drawBranch(window, end, angle - angleOffset, newLength, depth - 1);
        drawBranch(window, end, angle + angleOffset, newLength, depth - 1);
    }
public:
    FractalTree() : Obj(WIDTH, HEIGHT) {}
    void drawRec(RenderWindow& window, int depth) override { drawBranch(window, Vector2f(WIDTH / 2.f, HEIGHT - 50.f), -PI / 2.f, 100.f, depth); }
};

/////////////////////////////////////////////////////////////////////////////////////////////
class SierpinskiCarpet : public Obj {
private:
    void drawSquare(RenderWindow& window, Vector2f topLeft, float size, int depth) {
        if (depth == 0) {
            RectangleShape square(Vector2f(size, size));
            square.setPosition(topLeft);
            square.setFillColor(Color::White);
            window.draw(square);
            return;
        }

        float newSize = size / 3.f;
        for (int dx = 0; dx < 3; ++dx) {
            for (int dy = 0; dy < 3; ++dy) {
                if (dx == 1 && dy == 1)
                    continue;

                Vector2f newTopLeft = topLeft + Vector2f(dx * newSize, dy * newSize);
                drawSquare(window, newTopLeft, newSize, depth - 1);
            }
        }
    }
public:
    SierpinskiCarpet() : Obj(WIDTH, HEIGHT) {}
    void drawRec(RenderWindow& window, int depth) override {drawSquare(window, Vector2f(50.f, 50.f), WIDTH - 100.f, depth);}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
Obj* setobject() {
    enum b{ SperTriangl1 , KochSnowflake1, FractalTree1, SierpinskiCarpet1};
    int a = 0;
    
    cin >> a;
    switch (a-1)
    {
    case SperTriangl1: return new SperTriangl();
    case KochSnowflake1: return new KochSnowflake();
    case FractalTree1: return new FractalTree();
    case SierpinskiCarpet1: return new SierpinskiCarpet();
    default: return nullptr;
    }
}
int main() {
    SetConsoleOutputCP(1251);

    Obj* object = setobject();

    RenderWindow window(VideoMode({ WIDTH, HEIGHT }), "");

    View view = window.getDefaultView();
    float zoom_lvl = 1;

    int depth = 0;
    Clock clock;

    //

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
                }
        }

        //

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

        //

        if (clock.getElapsedTime().asSeconds() > 1.0f) {
            if (depth < maxDepth)
                depth++;
            clock.restart();
        }
        
        //
        if (object) {
            window.setView(view);
            window.clear();
            //

            object->drawRec(window, depth); 

            //
            window.display();
        }
        else {
            cout << "Не коректное значение" << endl; window.close(); break;
        }
    }
    delete object;
    main();
    return 0;
}
