#include <math.h>
#include <pango/pangocairo.h>
#include <pango/pango.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SDL2/SDL.h>

#define FONT "DejaVu Sans Mono 11"

static int width = 600, height = 600;

std::string slurp(const char *filename)
{
        std::ifstream in;
        in.open(filename, std::ifstream::in);
        std::stringstream sstr;
        sstr << in.rdbuf();
        in.close();
        return sstr.str();
}

static void
draw_text (cairo_t *cr)
{
    PangoLayout *layout;
    PangoFontDescription *desc;

    auto pc_context = pango_cairo_create_context(cr);
    layout = pango_layout_new (pc_context);
    // auto font_options = cairo_font_options_create();
    // cairo_font_options_set_hint_style(font_options, CAIRO_HINT_STYLE_FULL);
    // cairo_font_options_set_antialias(font_options, CAIRO_ANTIALIAS_SUBPIXEL);
    // cairo_font_options_set_hint_style(font_options, CAIRO_HINT_STYLE_NONE);
    // cairo_font_options_set_antialias(font_options, CAIRO_ANTIALIAS_NONE);
    // pango_cairo_context_set_font_options(pc_context, font_options);

    auto text = slurp("./lorem.txt");
    pango_layout_set_text (layout, text.c_str(), -1);
    desc = pango_font_description_from_string (FONT);
    pango_layout_set_font_description (layout, desc);
    pango_font_description_free (desc);

    pango_layout_set_width(layout, width * PANGO_SCALE);
    pango_layout_set_wrap(layout, PANGO_WRAP_WORD);

    cairo_save (cr);

    cairo_set_source_rgb (cr, 0, 0, 0);

    cairo_move_to (cr, 10, 0);
    // pango_cairo_context_set_font_options(pc_context, font_options);
    pango_cairo_show_layout (cr, layout);

    cairo_restore (cr);

    g_object_unref (layout);
    g_object_unref(pc_context);
    // cairo_font_options_destroy(font_options);

}

void draw_to_sdl(SDL_Surface *sdlsurf){
  cairo_t *cr;
  char *filename;
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
  draw_text (cr);
  cairo_destroy (cr);

  // status = cairo_surface_write_to_png (surface, filename);
  cairo_surface_destroy (surface);

  // if (status != CAIRO_STATUS_SUCCESS)
  //   {
  //     g_printerr ("Could not save png to '%s'\n", filename);
  //     return 1;
  //   }

}

SDL_Texture *create_texture(SDL_Renderer *ren){
  SDL_Surface *sdlsurf = SDL_CreateRGBSurface(0, width, height, 32,
    0x00FF0000, /* Rmask */
    0x0000FF00, /* Gmask */
    0x000000FF, /* Bmask */
    0);
  if (sdlsurf == nullptr){
    std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
    throw "oops";
  }

  draw_to_sdl(sdlsurf);


  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, sdlsurf);
  SDL_FreeSurface(sdlsurf);
  if (tex == nullptr){
    std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    throw "oops";
  }

  return tex;
}

int main (int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
  }

  SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, width, height, SDL_WINDOW_SHOWN);
  if (win == nullptr){
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
  SDL_SetWindowResizable(win, SDL_TRUE);

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr){
    SDL_DestroyWindow(win);
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }


  SDL_Texture *tex = create_texture(ren);

  bool quit = false;

  SDL_Event e;

  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

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
            tex = create_texture(ren);
            SDL_RenderClear(ren);
            //Draw the texture
            SDL_RenderCopy(ren, tex, NULL, NULL);
            //Update the screen
            SDL_RenderPresent(ren);
            break;
        }
      }
    }
    SDL_RenderClear(ren);
    //Draw the texture
    SDL_RenderCopy(ren, tex, NULL, NULL);
    //Update the screen
    SDL_RenderPresent(ren);
  }


  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
	return 0;

}
