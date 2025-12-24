#include "Puck.h"
#include <SDL3/SDL.h>

Puck::Puck(int init_x, int init_y, float init_vel_x, float init_vel_y, int init_radius, SDL_Texture* init_texture)
{
	x = init_x;
	y = init_y;
	vel_x = init_vel_x;
	vel_y = init_vel_y;
	radius = init_radius;
	texture = init_texture;
	rect = { x - radius, y - radius, radius * 2, radius * 2 };
}

int Puck::get_x() { return x; }
int Puck::get_y() { return y; }
float Puck::get_vel_x() { return vel_x; }
float Puck::get_vel_y() { return vel_y; }
int Puck::get_radius() { return radius; }
SDL_Texture* Puck::get_texture() { return texture; }
SDL_FRect Puck::get_rect() { 
	SDL_FRect rect;
	rect.x = x - radius;
	rect.y = y - radius;
	rect.w = radius * 2;
	rect.h = radius * 2;
	return rect; 
}

void Puck::set_x(int new_x) { x = new_x; }
void Puck::set_y(int new_y) { y = new_y; }
void Puck::set_vel_x(float new_vel_x) { vel_x = new_vel_x; }
void Puck::set_vel_y(float new_vel_y) { vel_y = new_vel_y; }
void Puck::set_radius(float new_radius) { radius = new_radius; }
void Puck::set_texture(SDL_Texture* new_texture) { texture = new_texture; }