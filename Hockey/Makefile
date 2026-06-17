main:
	g++ main.cpp Puck.o Rink.o Camera.o Player.o -o game `pkg-config --cflags --libs sdl3` -lSDL3_image -lSDL3_ttf -lSDL3_mixer -lm
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
