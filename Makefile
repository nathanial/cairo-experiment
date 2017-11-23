CXX = g++
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE) `pkg-config --cflags pango cairo pangocairo`
LDFLAGS = $(SDL_LIB) `pkg-config --libs pango cairo pangocairo`
EXE = cairo-experiment

all: $(EXE)

$(EXE): main.o
	$(CXX) $< $(LDFLAGS) -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
