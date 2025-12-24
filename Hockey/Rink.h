#pragma once
#include <SDL3/SDL.h>

class Rink
{

public:
	Rink(int x, int y, int width, int height, SDL_Texture* texture);
	int get_x();
	int get_y();
	int get_width();
	int get_height();
	SDL_Texture* get_texture();

	void set_x(int new_x);
	void set_y(int new_y);
	void set_width(int new_width);
	void set_height(int new_height);
	void set_texture(SDL_Texture* new_texture);

private:
	int x;
	int y;
	int width;
	int height;
	SDL_Texture* texture;

};