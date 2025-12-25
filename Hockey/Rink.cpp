#include "Rink.h"
#include <SDL3/SDL.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

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
		printf("Loaded rink mesh point: (%d, %d)\n", x, y);
    }

    fclose(file);
}