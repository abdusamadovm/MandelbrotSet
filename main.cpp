#include "ComplexPlane.h"

int main() {

    int desktopWidth = VideoMode::getDesktopMode().width;
    int desktopHeight = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(desktopWidth, desktopHeight), "Mandelbrot Set");

    ComplexPlane complexPlane(window.getSize().x, window.getSize().y);

    Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        return -1;
    }

    Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(Color::White);
    text.setPosition(10, 10);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    complexPlane.zoomIn();
                }
                else if (event.mouseButton.button == Mouse::Right) {
                    complexPlane.zoomOut();
                }
            }

            if (event.type == Event::MouseMoved) {
                complexPlane.setMouseLocation(Vector2i(event.mouseMove.x, event.mouseMove.y));
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }
        }

        complexPlane.updateRender();
        complexPlane.loadText(text);

        window.clear(Color::Black);
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}
