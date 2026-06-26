#include "Puck.h"
#include "Player.h"
#include <SDL3/SDL.h>
#include <cmath>

Puck::Puck(int init_x, int init_y, float init_vel_x, float init_vel_y, int init_radius, SDL_Texture* init_texture, Player* possess_player)
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
	possessing_player = possess_player;
}

Puck::Puck(int init_x, int init_y, int init_radius) {
	
	rel_x = init_x;
	rel_y = init_y;
	vel_x = 0;
	vel_y = 0;
	radius = init_radius;
	texture = NULL;
	screen_x = 0;
	screen_x = 0;
	rect = { screen_x, screen_y, radius * 2, radius * 2 };
	possessing_player = nullptr;
	
}

void Puck::update_position() {

	if (possessing_player != nullptr) {

		float player_center_x = possessing_player->get_rel_x() + possessing_player->get_radius();
		float player_center_y = possessing_player->get_rel_y() + possessing_player->get_radius();

		rel_x = player_center_x - radius + 1.5 * cos(possessing_player->get_screen_angle() * (3.14159f / 180.0f)) * (possessing_player->get_radius() + radius * 2);
		rel_y = player_center_y - radius + 1.5 * sin(possessing_player->get_screen_angle() * (3.14159f / 180.0f)) * (possessing_player->get_radius() + radius * 2);

		vel_x = possessing_player->get_vel_x();
		vel_y = possessing_player->get_vel_y();

		return;
	}
	else {

		rel_x += vel_x;
		rel_y += vel_y;

	}
}

void Puck::update_screen_position(int rink_x, int rink_y) {
	screen_x = rink_x + rel_x;
	screen_y = rink_y + rel_y;
}

int Puck::get_screen_x(int rink_x) { return rink_x + rel_x; }
int Puck::get_screen_y(int rink_y) { return rink_y + rel_y; }
float Puck::get_rel_x() { return rel_x; }
float Puck::get_rel_y() { return rel_y; }
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
int Puck::get_center_x() { return rel_x + radius; }
int Puck::get_center_y() { return rel_y + radius; }
float Puck::get_total_velocity() { return sqrt(vel_x * vel_x + vel_y * vel_y); }

void Puck::set_rel_x(float new_rel_x) { rel_x = new_rel_x; }
void Puck::set_rel_y(float new_rel_y) { rel_y = new_rel_y; }
void Puck::multiply_vel_x(float factor) { vel_x *= factor; }
void Puck::multiply_vel_y(float factor) { vel_y *= factor; }
void Puck::set_vel_x(float new_vel_x) { vel_x = new_vel_x; }
void Puck::set_vel_y(float new_vel_y) { vel_y = new_vel_y; }
void Puck::set_radius(int new_radius) { radius = new_radius; }
void Puck::set_texture(SDL_Texture* new_texture) { texture = new_texture; }
