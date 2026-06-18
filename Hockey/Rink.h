#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <cmath>
#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Rink
{

public:
	Rink();
	Rink(int x, int y, int width, int height, SDL_Texture* texture);
	int get_screen_x(int camera_x);
	int get_screen_y(int camera_y);
	int get_width();
	int get_height();
	SDL_Texture* get_texture();

	void set_screen_x(int new_x);
	void set_screen_y(int new_y);
	void set_width(int new_width);
	void set_height(int new_height);
	void set_texture(SDL_Texture* new_texture);

private:
	float screen_x;
	float screen_y;
	int width;
	int height;
	SDL_Texture* texture;

};
