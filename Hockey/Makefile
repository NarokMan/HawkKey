main:
	g++ main.cpp Puck.o Rink.o Camera.o Player.o -o game `pkg-config --cflags --libs sdl3` -lSDL3_image -lSDL3_ttf -lSDL3_mixer -lm
map:
	g++ -c Map.cpp
camera:
	g++ -c Camera.cpp
rink:
	g++ -c Rink.cpp `pkg-config --cflags --libs sdl3` -lm
puck:
	g++ -c Player.cpp Puck.cpp `pkg-config --cflags --libs sdl3` -lm
player:
	g++ -c Player.cpp Puck.cpp `pkg-config --cflags --libs sdl3` -lm
clean:
	rm *.o
all:
	g++ -c Map.cpp
	g++ -c Camera.cpp
	g++ -c Rink.cpp `pkg-config --cflags --libs sdl3` -lm
	g++ -c Player.cpp Puck.cpp `pkg-config --cflags --libs sdl3` -lm
	g++ main.cpp Puck.o Rink.o Camera.o Player.o Map.o -o game `pkg-config --cflags --libs sdl3` -lSDL3_image -lSDL3_ttf -lSDL3_mixer -lm
