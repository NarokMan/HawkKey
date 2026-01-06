#pragma once
#include <SDL3/SDL.h>
#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

class Rink
{

public:
	Rink(int x, int y, int width, int height, SDL_Texture* texture);
	int get_screen_x(int camera_x);
	int get_screen_y(int camera_y);
	int get_width();
	int get_height();
	SDL_Texture* get_texture();
	std::vector<SDL_Point> get_rink_mesh();
	SDL_Point get_rink_mesh_point(int index);

	void set_screen_x(int new_x);
	void set_screen_y(int new_y);
	void set_width(int new_width);
	void set_height(int new_height);
	void set_texture(SDL_Texture* new_texture);

	void load_rink_mesh_from_file(const char* filename);
	bool check_rink_mesh_collision(int id, int puck_x, int puck_y, int puck_radius);
	float get_normal(int id);

private:
	float screen_x;
	float screen_y;
	int width;
	int height;
	SDL_Texture* texture;
	std::vector<SDL_Point> rink_mesh;

};