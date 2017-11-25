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
  FileWatcher watcher;
  Program p {};
  auto xml = slurp("./pinta.xml");
  p.loadUserInterfaceFromXML(const_cast<char*>(xml.c_str()));

  watcher.addWatch("./pinta.xml", [&](){
    p.reloadWidgetsFromXML(slurp("./pinta.xml"));
  });

  p.run();
	return 0;

}
