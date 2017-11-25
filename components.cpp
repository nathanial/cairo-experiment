#include "components.hpp"
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include "rapidxml/rapidxml.hpp"

const Margin& Widget::margin(){
    return _margin;
}

void Widget::setMargin(const Margin &margin){
    this->_margin = margin;
}

void Widget::render(Graphics &graphics) {
    this->paint(graphics);
}

Button::Button(const std::string &text): _text(text) {

}

void Button::paint(Graphics &graphics) {
    graphics.setAntialias(false);
    graphics.setLineWidth(1);

    graphics.setColor(Colors::White);
    graphics.fillRectangle(0,0,100,30);

    graphics.setColor(Colors::Black);
    graphics.drawRectangle(0,0,100,30);

    graphics.drawText(0,0,100,30, this->_text, PANGO_ALIGN_CENTER);
}

double Button::width(){
    return 100;
}

double Button::height() {
    return 30;
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

void CairoGraphics::save(){
    cairo_save(this->_cr);
}

void CairoGraphics::restore(){
    cairo_restore(this->_cr);
}

void CairoGraphics::translate(double x, double y) {
    cairo_translate(this->_cr, x, y);
}

Window::Window(const std::string &title, int width, int height) : title(title), width(width), height(height) {

}

Window::~Window(){
    if(_window != nullptr){
        SDL_DestroyTexture(this->_texture);
        SDL_DestroyRenderer(this->_renderer);
        SDL_DestroyWindow(this->_window);
        SDL_Quit();
    }
}

void Window::open() {
    this->_window = SDL_CreateWindow(title.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN);
    if (this->_window == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        throw "could not open window";
    }
    SDL_SetWindowResizable(this->_window, SDL_TRUE);
    this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (this->_renderer == nullptr){
        SDL_DestroyWindow(this->_window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        throw "could not create renderer";
    }

    SDL_Texture *tex = create_texture();

    bool quit = false;

    SDL_Event e;

    SDL_SetRenderDrawColor(this->_renderer, 255, 255, 255, 255);
    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
    while(!quit){
        while (SDL_PollEvent(&e)){
            //If user closes the window
            if (e.type == SDL_QUIT){
            quit = true;
            } else if(e.type == SDL_WINDOWEVENT) {
            switch(e.window.event){
                case SDL_WINDOWEVENT_RESIZED:
                width = e.window.data1;
                height = e.window.data2;
                SDL_DestroyTexture(tex);
                tex = this->create_texture();
                SDL_RenderClear(this->_renderer);
                //Draw the texture
                SDL_RenderCopy(this->_renderer, tex, NULL, NULL);
                //Update the screen
                SDL_RenderPresent(this->_renderer);
                break;
            }
            }
        }
        SDL_RenderClear(this->_renderer);
        //Draw the texture
        SDL_RenderCopy(this->_renderer, tex, NULL, NULL);
        //Update the screen
        SDL_RenderPresent(this->_renderer);
    }
}

void Window::addWidget(std::shared_ptr<Widget> widget) {
    this->_widgets.push_back(widget);
}


SDL_Texture *Window::create_texture(){
    SDL_Renderer *ren = this->_renderer;
    SDL_Surface *sdlsurf = SDL_CreateRGBSurface(0, width, height, 32,
        0x00FF0000, /* Rmask */
        0x0000FF00, /* Gmask */
        0x000000FF, /* Bmask */
        0);
    if (sdlsurf == nullptr){
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        throw "oops";
    }

    this->draw_to_sdl(sdlsurf);

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, sdlsurf);
    SDL_FreeSurface(sdlsurf);
    if (tex == nullptr){
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        throw "oops";
    }

    return tex;
}


void Window::draw_to_sdl(SDL_Surface *sdlsurf){
    cairo_t *cr;
    cairo_status_t status;
    cairo_surface_t *surface;

    surface = cairo_image_surface_create_for_data ( (unsigned char*)sdlsurf->pixels,
    CAIRO_FORMAT_RGB24,
    sdlsurf->w,
    sdlsurf->h,
    sdlsurf->pitch);
    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
    cairo_paint (cr);
    CairoGraphics g(cr);

    for(auto && widget: this->_widgets){
        widget->render(g);
    }

    cairo_destroy (cr);

    cairo_surface_destroy (surface);
}


void Widget::addWidget(std::shared_ptr<Widget> child){
    this->children.push_back(child);
}

void VerticalPanel::paint(Graphics &graphics) {
    int cursorY = 0;
    for(auto &&widget : this->children){
        graphics.save();
        graphics.translate(widget->margin().left, cursorY + widget->margin().top);
        widget->render(graphics);
        cursorY += widget->height() + widget->margin().bottom + widget->margin().top;
        graphics.restore();
    }
}

double VerticalPanel::height(){
    double total = 0;
    for(auto &&widget : this->children){
        total += widget->height() + widget->margin().top + widget->margin().bottom;
    }
    return total;
}

double VerticalPanel::width() {
    double max = 0;
    for(auto &&widget : this->children){
        auto width = widget->width() + widget->margin().left + widget->margin().right;
        if(width > max){
            width = max;
        }
    }
    return max;
}

using namespace rapidxml;
void Program::loadUserInterfaceFromXML(char *xml) {
    this->window = new Window("Program", 600, 600);
    xml_document<> doc;    // character type defaults to char
    doc.parse<0>(xml);    // 0 means default parse flags
    auto root = doc.first_node();
    auto xmlCursor = root->first_node();
    while(xmlCursor){
        auto child = this->createWidgetFromNode(xmlCursor);
        if(child){
            this->window->addWidget(child);
        }
        xmlCursor = xmlCursor->next_sibling();
    }
}

void Program::run(){
    this->window->open();
}

Program::~Program(){
    if(this->window != nullptr){
        delete this->window;
    }
}

std::shared_ptr<Widget> Program::createWidgetFromNode(rapidxml::xml_node<char>* node) {
    std::string name = std::string(node->name());
    std::shared_ptr<Widget> result;
    if(name == "VerticalPanel"){
        result = std::make_shared<VerticalPanel>();
    } else if(name == "Button") {
        result = std::make_shared<Button>();
    } else if(name == "MainMenu"){
        result = std::make_shared<MainMenu>();
    } else if(name == "Menu"){
        result = std::make_shared<Menu>();
    } else {
        std::cerr << "Unknown Widget: " << name << std::endl;
        return nullptr;
    }

    auto xmlCursor = node->first_node();
    while(xmlCursor){
        auto child = this->createWidgetFromNode(xmlCursor);
        if(child){
            result->addWidget(child);
        }
        xmlCursor = xmlCursor->next_sibling();
    }
    return result;
}


double MainMenu::width() {
    return 0;
}

double MainMenu::height(){
    return 0;
}

void MainMenu::paint(Graphics &graphics) {

}


double Menu::width() {
    return 0;
}

double Menu::height(){
    return 0;
}

void Menu::paint(Graphics &graphics) {

}
