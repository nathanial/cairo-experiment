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


CairoGraphics::CairoGraphics(cairo_t *cr) : _cr(cr) {

}

void CairoGraphics::setColor(const Color &color)  {
    cairo_set_source_rgba(this->_cr, color.r, color.g, color.b, color.a);
}

void CairoGraphics::setAntialias(bool value) {
    if(value){
        cairo_set_antialias(this->_cr, CAIRO_ANTIALIAS_BEST);
    } else {
        cairo_set_antialias(this->_cr, CAIRO_ANTIALIAS_NONE);
    }
}

void CairoGraphics::fillRectangle(int x, int y, int width, int height) {
    cairo_rectangle(this->_cr, x, y, width, height);
    cairo_fill(this->_cr);
}

void CairoGraphics::drawRectangle(int x, int y, int width, int height) {
    cairo_rectangle(this->_cr, x, y, width, height);
    cairo_stroke(this->_cr);
}

void CairoGraphics::setLineWidth(int width) {
    cairo_set_line_width(this->_cr, width);
}
