#include "components.hpp"


Button::Button(const std::string &text): _text(text) {

}

void Button::draw(Graphics &graphics) {
    graphics.setAntialias(false);
    graphics.setLineWidth(1);

    graphics.setColor(Colors::White);
    graphics.fillRectangle(0,0,100,30);

    graphics.setColor(Colors::Black);
    graphics.drawRectangle(0,0,100,30);
}

