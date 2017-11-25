#include <math.h>
#include <pango/pangocairo.h>
#include <pango/pango.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <SDL2/SDL.h>
#include "components.hpp"
#define FONT "DejaVu Sans Mono 11"

int main (int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
  }

  Window mainWindow("Demo", 600, 600);
  auto panel = std::make_shared<VerticalPanel>();
  panel->addWidget(std::make_shared<Button>("Ok"));
  panel->addWidget(std::make_shared<Button>("Cancel"));
  mainWindow.addWidget(panel);
  mainWindow.open();

	return 0;

}
