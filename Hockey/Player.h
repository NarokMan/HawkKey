#pragma once
#include <SDL3/SDL.h>

enum player_states {
	POSSESSING = 0,
	NOT_POSSESSING_STABBING = 1,
	NOT_POSSESSING_NOT_STABBING = 2
};

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
	int get_center_x();
	int get_center_y();
	player_states get_player_state();
	bool colliding_with_puck(int puck_x, int puck_y, int puck_radius);

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

	SDL_Point center;
	int radius;

	player_states player_state;

	SDL_Texture* texture;

};