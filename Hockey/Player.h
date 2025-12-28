#pragma once
#include <SDL3/SDL.h>

class Player
{

public:
	Player(int start_x, int start_y, int r, float angle, SDL_Texture* tex);

	void update_position();
	void update_screen_position(int rink_x, int rink_y);

	int get_screen_x(int rink_x);
	int get_screen_y(int rink_y);
	float get_rel_x();
	float get_rel_y();
	float get_vel_x();
	float get_vel_y();
	int get_radius();
	SDL_Texture* get_texture();
	float get_screen_angle();
	SDL_FRect get_rect();

	void set_rel_x(float new_rel_x);
	void set_rel_y(float new_rel_y);
	void set_vel_x(float new_vel_x);
	void set_vel_y(float new_vel_y);
	void set_screen_angle(float new_angle);


private:
	int screen_x;
	int screen_y;
	SDL_FRect rect;

	float rel_x;
	float rel_y;

	float vel_x;
	float vel_y;

	float screen_angle;

	int radius;

	SDL_Texture* texture;

};