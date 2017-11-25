#include "components.hpp"
#include <iostream>


Button::Button(const std::string &text): _text(text) {

}

void Button::draw(Graphics &graphics) {
    graphics.setAntialias(false);
    graphics.setLineWidth(1);

    graphics.setColor(Colors::White);
    graphics.fillRectangle(0,0,100,30);

    graphics.setColor(Colors::Black);
    graphics.drawRectangle(0,0,100,30);

    graphics.drawText(0,0,100,30, this->_text, PANGO_ALIGN_CENTER);
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

void CairoGraphics::drawText(int x, int y, int width, int height, const std::string &text, PangoAlignment alignment) {
    PangoLayout *layout;
    PangoFontDescription *desc;
    auto font = "DejaVu Sans Mono 11";
    auto cr = this->_cr;

    cairo_save (cr);
    auto pc_context = pango_cairo_create_context(cr);
    layout = pango_layout_new (pc_context);
    auto font_options = cairo_font_options_create();
    cairo_font_options_set_hint_style(font_options, CAIRO_HINT_STYLE_FULL);
    cairo_font_options_set_antialias(font_options, CAIRO_ANTIALIAS_SUBPIXEL);
    pango_cairo_context_set_font_options(pc_context, font_options);

    pango_layout_set_text (layout, text.c_str(), -1);
    desc = pango_font_description_from_string (font);
    pango_layout_set_font_description (layout, desc);
    pango_font_description_free (desc);

    pango_layout_set_width(layout, width * PANGO_SCALE);
    pango_layout_set_wrap(layout, PANGO_WRAP_WORD);
    pango_layout_set_alignment(layout, alignment);
    int layoutWidth;
    int layoutHeight;
    pango_layout_get_pixel_size(layout, &layoutWidth, &layoutHeight);

    auto topMargin = (height - layoutHeight) / 2;

    cairo_set_source_rgb (cr, 0, 0, 0);

    cairo_move_to (cr, x, y + topMargin);
    pango_cairo_show_layout (cr, layout);

    cairo_restore (cr);

    g_object_unref (layout);
    g_object_unref(pc_context);
}
