#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

#include <pango/pangocairo.h>
#include <pango/pango.h>
#include <string>

struct Color {
    float r;
    float g;
    float b;
    float a;
    Color(float r, float g, float b, float a) : r(r),g(g),b(b),a(a) {}
};

namespace Colors {
    static const Color White = Color(1,1,1,1);
    static const Color Black = Color(0,0,0,1);
};

class Graphics {
public:
    virtual void setColor(const Color &color) = 0;
    virtual void setAntialias(bool value) = 0;
    virtual void fillRectangle(int x, int y, int width, int height) = 0;
    virtual void drawRectangle(int x, int y, int width, int height) = 0;
    virtual void setLineWidth(int width) = 0;
};

class CairoGraphics : public Graphics {
private:
    cairo_t *_cr;
public:
    CairoGraphics(cairo_t *cr);
    void setColor(const Color &color) override;
    void setAntialias(bool value) override;
    void fillRectangle(int x, int y, int width, int height) override;
    void drawRectangle(int x, int y, int width, int height) override;
    void setLineWidth(int width) override;
};

class Widget {
public:
    virtual void draw(Graphics &graphics) = 0;

protected:
    void drawChildren();
};

class Button : public Widget {
private:
    std::string _text;

public:
    Button(const std::string &text);
    void draw(Graphics &graphics) override;
};

#endif
