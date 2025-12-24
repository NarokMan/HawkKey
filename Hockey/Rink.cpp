#include "Rink.h"
#include <SDL3/SDL.h>

Rink::Rink(int init_x, int init_y, int init_width, int init_height, SDL_Texture* init_texture)
{
	screen_x = init_x;
	screen_y = init_y;
	width = init_width;
	height = init_height;
	texture = init_texture;
}

int Rink::get_screen_x() { return screen_x; }
int Rink::get_screen_y() { return screen_y; }
int Rink::get_width() { return width; }
int Rink::get_height() { return height; }
SDL_Texture* Rink::get_texture() { return texture; }

void Rink::set_screen_x(int new_x) { screen_x = new_x; }
void Rink::set_screen_y(int new_y) { screen_y = new_y; }
void Rink::set_width(int new_width) { width = new_width; }
void Rink::set_height(int new_height) { height = new_height; }
void Rink::set_texture(SDL_Texture* new_texture) { texture = new_texture; }