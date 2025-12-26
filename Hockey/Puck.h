#pragma once
#include <SDL3/SDL.h>

class Puck
{

public:

	Puck(int start_x, int start_y, float start_vel_x, float start_vel_y, int start_radius, SDL_Texture* start_texture);
	void update_position();
	void update_screen_position(int rink_x, int rink_y);

	bool check_collision_with_rink(int x1, int y1, int x2, int y2);

	int get_screen_x(int rink_x);
	int get_screen_y(int rink_y);
	float get_rel_x();
	float get_rel_y();
	float get_vel_x();
	float get_vel_y();
	int get_radius();
	SDL_Texture* get_texture();
	SDL_FRect get_rect();
	int get_center_x();
	int get_center_y();
	float get_total_velocity();

	void set_rel_x(float new_rel_x);
	void set_rel_y(float new_rel_y);
	void multiply_vel_x(float factor);
	void multiply_vel_y(float factor);
	void set_vel_x(float new_vel_x);
	void set_vel_y(float new_vel_y);
	void set_radius(int new_radius);
	void set_texture(SDL_Texture* new_texture);

private:
	int screen_x; // On-screen x position
	int screen_y; // On-screen y position
	float rel_x; // x position relative to the top left corner of the rink image
	float rel_y; // y position relative to the top left corner of the rink image
	float vel_x; // Velocity in the x direction
	float vel_y; // Velocity in the y direction
	int radius;
	SDL_Texture* texture;
	SDL_Rect rect;
};

