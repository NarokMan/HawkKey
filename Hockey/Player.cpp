#include "Player.h"
#include <SDL3/SDL.h>

Player::Player(int start_x, int start_y, int r, float angle, SDL_Texture* tex)
{
	texture = tex;
	rel_x = start_x;
	rel_y = start_y;
	radius = r;
	screen_x = 0;
	screen_y = 0;
	vel_x = 0.0f;
	vel_y = 0.0f;
	screen_angle = angle;
	rect.x = screen_x;
	rect.y = screen_y;
	rect.w = radius * 2;
	rect.h = radius * 2;
}

void Player::update_position() {
	rel_x += vel_x;
	rel_y += vel_y;
}

void Player::update_screen_position(int rink_x, int rink_y) {
	screen_x = rink_x + rel_x;
	screen_y = rink_y + rel_y;
}

int Player::get_screen_x(int rink_x) { return rink_x + rel_x; }
int Player::get_screen_y(int rink_y) { return rink_y + rel_y; }
float Player::get_rel_x() { return rel_x; }
float Player::get_rel_y() { return rel_y; }
float Player::get_vel_x() { return vel_x; }
float Player::get_vel_y() { return vel_y; }
int Player::get_radius() { return radius; }
SDL_Texture* Player::get_texture() { return texture; }
float Player::get_screen_angle() { return screen_angle; }
SDL_FRect Player::get_rect() {
	SDL_FRect updated_rect;
	updated_rect.x = screen_x;
	updated_rect.y = screen_y;
	updated_rect.w = radius * 2;
	updated_rect.h = radius * 2;
	return updated_rect;
}

void Player::set_rel_x(float new_rel_x) { rel_x = new_rel_x; }
void Player::set_rel_y(float new_rel_y) { rel_y = new_rel_y; }
void Player::set_vel_x(float new_vel_x) { vel_x = new_vel_x; }
void Player::set_vel_y(float new_vel_y) { vel_y = new_vel_y; }
void Player::set_screen_angle(float new_angle) { screen_angle = new_angle; }