#include "Rink.h"
#include <SDL3/SDL.h>

Rink::Rink(int init_x, int init_y, int init_width, int init_height, SDL_Texture* init_texture)
{
	screen_x = init_x;
	screen_y = init_y;
	width = init_width;
	height = init_height;
	texture = init_texture;
}

int Rink::get_screen_x(int camera_x) { return screen_x - camera_x + WINDOW_WIDTH / 2; }
int Rink::get_screen_y(int camera_y) { return screen_y - camera_y + WINDOW_HEIGHT / 2; }
int Rink::get_width() { return width; }
int Rink::get_height() { return height; }
SDL_Texture* Rink::get_texture() { return texture; }
std::vector <SDL_Point> Rink::get_rink_mesh() { return rink_mesh; }
SDL_Point Rink::get_rink_mesh_point(int index) { return rink_mesh[index]; }

void Rink::set_screen_x(int new_x) { screen_x = new_x; }
void Rink::set_screen_y(int new_y) { screen_y = new_y; }
void Rink::set_width(int new_width) { width = new_width; }
void Rink::set_height(int new_height) { height = new_height; }
void Rink::set_texture(SDL_Texture* new_texture) { texture = new_texture; }

void Rink::load_rink_mesh_from_file(const char* filename)
{
    rink_mesh.clear();
    FILE* file = NULL;

    // fopen_s returns errno_t (0 on success)
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0 || file == NULL) {
        SDL_Log("Failed to open rink mesh file: %s", filename);
        return;
    }

    int x, y;
    while (fscanf_s(file, "%d,%d", &x, &y) == 2) {
        SDL_Point point = { x, y };
        rink_mesh.push_back(point);
    }

    fclose(file);
}

// Checks collision between mesh line segment with id 'id' and a puck WITH CENTER puck_x, puck_y and radius puck_radius
// Returns true if puck overlaps the line
bool Rink::check_rink_mesh_collision(int id, int puck_x, int puck_y, int puck_radius)
{
    int id_1 = id;
	int id_2 = (id + 1) % rink_mesh.size();

	// Vector from point 1 to point 2
	float dx = rink_mesh[id_2].x - rink_mesh[id_1].x;
	float dy = rink_mesh[id_2].y - rink_mesh[id_1].y;

	// Vector from point 1 to puck center
	float fx = puck_x - rink_mesh[id_1].x;
	float fy = puck_y - rink_mesh[id_1].y;

    float t = (fx * dx + fy * dy) / (dx * dx + dy * dy);

    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;

	float closest_x = rink_mesh[id_1].x + t * dx;
	float closest_y = rink_mesh[id_1].y + t * dy;

	float dist_x = puck_x - closest_x;
	float dist_y = puck_y - closest_y;

	float distance_squared = dist_x * dist_x + dist_y * dist_y;

    return distance_squared <= puck_radius * puck_radius;
}

float get_regular_func(float angle) {

    while (angle > 3.141592653f) {
		angle -= 2.0f * 3.141592653f;
    }

    while (angle < -3.141592653f) {
        angle += 2.0f * 3.141592653f;
    }

	return angle;

}

float Rink::get_normal(int id) {

    int id_1 = id;
    int id_2 = (id + 1) % rink_mesh.size();

    // Vector from point 1 to point 2
    float dx = rink_mesh[id_2].x - rink_mesh[id_1].x;
    float dy = rink_mesh[id_2].y - rink_mesh[id_1].y;

	// Normal vector
    float normal_x = -dy;
    float normal_y = dx;

	float angle = atan2f(normal_y, normal_x); // Angle in radians
	angle = get_regular_func(angle);

	return angle;

}