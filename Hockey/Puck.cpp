#include "Puck.h"
#include <SDL3/SDL.h>

Puck::Puck(int init_x, int init_y, float init_vel_x, float init_vel_y, int init_radius, SDL_Texture* init_texture)
{
	rel_x = init_x;
	rel_y = init_y;
	vel_x = init_vel_x;
	vel_y = init_vel_y;
	radius = init_radius;
	texture = init_texture;
	screen_x = 0;
	screen_y = 0;
	rect = { screen_x, screen_y, radius * 2, radius * 2 };
}

void Puck::update_position() {
	rel_x += vel_x;
	rel_y += vel_y;
}

void Puck::update_screen_position(int rink_x, int rink_y) {
	screen_x = rink_x + rel_x;
	screen_y = rink_y + rel_y;
}

int Puck::get_screen_x(int rink_x) { return rink_x + rel_x; }
int Puck::get_screen_y(int rink_y) { return rink_y + rel_y; }
float Puck::get_vel_x() { return vel_x; }
float Puck::get_vel_y() { return vel_y; }
int Puck::get_radius() { return radius; }
SDL_Texture* Puck::get_texture() { return texture; }
SDL_FRect Puck::get_rect() { 
	SDL_FRect rect;
	rect.x = screen_x;
	rect.y = screen_y;
	rect.w = radius * 2;
	rect.h = radius * 2;
	return rect; 
}

void Puck::set_vel_x(float new_vel_x) { vel_x = new_vel_x; }
void Puck::set_vel_y(float new_vel_y) { vel_y = new_vel_y; }
void Puck::set_radius(float new_radius) { radius = new_radius; }
void Puck::set_texture(SDL_Texture* new_texture) { texture = new_texture; }