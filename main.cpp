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

std::string slurp(const char *filename)
{
  std::ifstream in;
  in.open(filename, std::ifstream::in);
  std::stringstream sstr;
  sstr << in.rdbuf();
  in.close();
  return sstr.str();
}

int main (int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
  }

  Program p {};
  auto xml = slurp("./pinta.xml");
  p.loadUserInterfaceFromXML(const_cast<char*>(xml.c_str()));
  p.run();
	return 0;

}
