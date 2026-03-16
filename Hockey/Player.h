#pragma once
#include <SDL3/SDL.h>

enum player_states {
	POSSESSING = 0,
	NOT_POSSESSING_STABBING = 1,
	NOT_POSSESSING_NOT_STABBING = 2
};

class Puck;

class Player
{

public:
	Player(int start_x, int start_y, int r, float angle, SDL_Texture* tex, Puck* possessing_puck);

	void update_position();
	void update_screen_position(int rink_x, int rink_y);
	void update_stick_angle();

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
	bool stick_colliding_with_puck(int x1, int y1, int x2, int y2, int puck_x, int puck_y, int puck_radius);

	//bool is_left_of_stick(int puck_center_x, int puck_center_y);
	bool is_facing_puck(float angle_to_puck, float angle_threshold);

	void set_rel_x(float new_rel_x);
	void set_rel_y(float new_rel_y);
	void set_vel_x(float new_vel_x);
	void set_vel_y(float new_vel_y);
	void set_target_angle(float new_angle);
	void set_player_state(player_states player_state);

	Puck* possessed_puck;

private:
	int screen_x;
	int screen_y;
	SDL_FRect rect;

	float rel_x;
	float rel_y;

	float vel_x;
	float vel_y;

	float screen_angle;
	float target_angle;

	int radius;

	player_states player_state;

	SDL_Texture* texture;

};