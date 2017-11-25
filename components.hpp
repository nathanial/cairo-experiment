#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

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
    void setColor(const Color &color);
    void setAntialias(bool value);
    void fillRectangle(int x, int y, int width, int height);
    void drawRectangle(int x, int y, int width, int height);
    void setLineWidth(int width);
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
