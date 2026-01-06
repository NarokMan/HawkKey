#include "Player.h"
#include "Puck.h"
#include <SDL3/SDL.h>

Player::Player(int start_x, int start_y, int r, float angle, SDL_Texture* tex, Puck* possessing_puck)
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
	rect.w = this->radius * 2;
	rect.h = this->radius * 2;
	player_state = NOT_POSSESSING_NOT_STABBING;
	possessed_puck = possessing_puck;
	target_angle = angle;
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
player_states Player::get_player_state() { return player_state; }
int Player::get_center_x() { return rel_x + radius; }
int Player::get_center_y() { return rel_y + radius; }
bool Player::colliding_with_puck(int puck_x, int puck_y, int puck_radius) {
	float dx = (rel_x + radius) - (puck_x);
	float dy = (rel_y + radius) - (puck_y);
	float distance = sqrt(dx * dx + dy * dy);
	if (distance < radius + puck_radius) {
		return true;
	}
	return false;
}

bool Player::stick_colliding_with_puck(int x1, int y1, int x2, int y2, int puck_x, int puck_y, int puck_radius) {

	// Vector from point 1 to point 2
	float dx = x2 - x1;
	float dy = y2 - y1;

	// Vector from point 1 to puck center
	float fx = puck_x - x1;
	float fy = puck_y - y1;

	float t = (fx * dx + fy * dy) / (dx * dx + dy * dy);

	if (t < 0.0f)
		t = 0.0f;
	else if (t > 1.0f)
		t = 1.0f;

	float closest_x = x1 + t * dx;
	float closest_y = y1 + t * dy;

	float dist_x = puck_x - closest_x;
	float dist_y = puck_y - closest_y;

	float distance_squared = dist_x * dist_x + dist_y * dist_y;

	return distance_squared <= puck_radius * puck_radius * 4;
}

// Random cross product thing I found on stack overflow
bool Player::is_left_of_stick(int puck_center_x, int puck_center_y) {

	float stick_angle = screen_angle * (3.14159f / 180.0f);
	float stick_dx = cos(stick_angle);
	float stick_dy = sin(stick_angle);

	float to_puck_x = puck_center_x - get_center_x();
	float to_puck_y = puck_center_y - get_center_y();

	float cross = stick_dx * to_puck_y - stick_dy * to_puck_x;

	return cross > 0;
}

int angle_turn_direction(float screen_angle, float target_angle) {
	// Calculate shortest angular difference
	float diff = target_angle - screen_angle;
	// Wrap difference
	while (diff > 180) diff -= 2 * 180;
	while (diff < -180) diff += 2 * 180;

	// If diff is positive, turn counter-clockwise (increase angle)
	// If diff is negative, turn clockwise (decrease angle)
	if (diff > 3.0f) return 1;   // Increase (turn left)
	if (diff < -3.0f) return -1; // Decrease (turn right)
	return 0; // Close enough, no turn needed
}

void Player::update_stick_angle() {

	if (angle_turn_direction(screen_angle, target_angle) == 1) {
		screen_angle += 4.0f; // Turn left
	}
	else if (angle_turn_direction(screen_angle, target_angle) == -1) {
		screen_angle -= 4.0f; // Turn right
	}

}

void Player::set_rel_x(float new_rel_x) { rel_x = new_rel_x; }
void Player::set_rel_y(float new_rel_y) { rel_y = new_rel_y; }
void Player::set_vel_x(float new_vel_x) { vel_x = new_vel_x; }
void Player::set_vel_y(float new_vel_y) { vel_y = new_vel_y; }
void Player::set_target_angle(float new_angle) { target_angle = new_angle; }
void Player::set_player_state(player_states new_state) { player_state = new_state; }