#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Puck.h"
#include "Rink.h"
#include "Camera.h"
#include <time.h>
#include <vector>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

#define TARGET_FPS 75

struct controls {

    bool up = false;
    bool down = false;
    bool left = false;
	bool right = false;

};

struct controls buttons = { false, false, false, false };

 /* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

int num_textures = 2;
std::vector<SDL_Texture*> textures;

Rink rink(0, 0, 3000, 1275, NULL);

int num_pucks = 2;
std::vector<Puck> pucks;

Camera camera;

int frameStart = 0;

MIX_Mixer* mixer = NULL;

/*  This function runs once at startup. 
    Its parameters are:
    'appstate', which gives an optional space to store a pointer for future use
    'argc', which is just normal argc
    'argv' which is just normal argv */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{

    SDL_SetAppMetadata("SDL3 Window", "0.9.0", "com.narok.template"); // Sets app metadata

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log(ANSI_COLOR_RED "Couldn't initialize SDL: %s" ANSI_COLOR_RESET, SDL_GetError());
        return SDL_APP_FAILURE;
    }
    else {
        SDL_Log(ANSI_COLOR_GREEN "Successfully initialized SDL!" ANSI_COLOR_RESET);
    }

    // Initialize Mixer
    if (!MIX_Init()) {
        SDL_Log(ANSI_COLOR_RED "Couldn't initialize SDL_Mixer: %s" ANSI_COLOR_RESET, SDL_GetError());
        return SDL_APP_FAILURE;
    }
    else {
        SDL_Log(ANSI_COLOR_GREEN "Initialized SDL_Mixer!" ANSI_COLOR_RESET);
    }

    // Initialize ttf
    if (!TTF_Init()) {
        SDL_Log(ANSI_COLOR_RED "Couldn't initialize SDL_ttf: %s" ANSI_COLOR_RESET, SDL_GetError());
        return SDL_APP_FAILURE;
    }
    else {
        SDL_Log(ANSI_COLOR_GREEN "Initialized SDL_ttf!" ANSI_COLOR_RESET);
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (mixer == NULL) {
        SDL_Log(ANSI_COLOR_RED "Couldn't initialize the mixer: %s" ANSI_COLOR_RESET, SDL_GetError());
        return SDL_APP_FAILURE;
    }
    else {
        SDL_Log(ANSI_COLOR_GREEN "Initialized the mixer!" ANSI_COLOR_RESET);
    }

    // Create window
    if (!SDL_CreateWindowAndRenderer("SDL3 Window", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log(ANSI_COLOR_RED "Couldn't create window/renderer: %s" ANSI_COLOR_RESET, SDL_GetError());
        return SDL_APP_FAILURE;
    }
    else {
        SDL_Log(ANSI_COLOR_GREEN "Created a window!" ANSI_COLOR_RESET);
    }

	// Enable VSync
    SDL_SetRenderVSync(renderer, 1);

    // Sets a logical presentation for the renderer. This is super awesome and cool.
    // It scales stuff for the DPI screens and stuff
    if (!SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
        SDL_Log(ANSI_COLOR_RED "SetRenderLogicalPresentation() failed: %s" ANSI_COLOR_RESET, SDL_GetError());
        return SDL_APP_FAILURE;
    }
    else {
        SDL_Log(ANSI_COLOR_GREEN "SetRenderLogicalPresentation() succeeded!" ANSI_COLOR_RESET);
    }

	textures.resize(num_textures);

	SDL_Surface* temp_surface = SDL_LoadPNG("thingies/Eyes/puck4.png");
    if (temp_surface == NULL) {
        SDL_Log(ANSI_COLOR_RED "Failed to load puck image: %s" ANSI_COLOR_RESET, SDL_GetError());
		textures[0] = NULL;
    } else {
        SDL_Log(ANSI_COLOR_GREEN "Loaded puck image successfully!" ANSI_COLOR_RESET);
        textures[0] = SDL_CreateTextureFromSurface(renderer, temp_surface);
        SDL_DestroySurface(temp_surface);
	}

    temp_surface = SDL_LoadPNG("thingies/Eyes/rink5.png");
    if (temp_surface == NULL) {
        SDL_Log(ANSI_COLOR_RED "Failed to load rink image: %s" ANSI_COLOR_RESET, SDL_GetError());
        textures[1] = NULL;
    }
    else {
        SDL_Log(ANSI_COLOR_GREEN "Loaded rink image successfully!" ANSI_COLOR_RESET);
        textures[1] = SDL_CreateTextureFromSurface(renderer, temp_surface);
        SDL_DestroySurface(temp_surface);
    }

    srand(time(NULL));

    for (int i = 0; i < num_pucks; i++) {
        pucks.push_back(Puck(rand() % 3000, rand() % 1000, rand() % 100 / 10 + 1, rand() % 100 / 10 + 1, 10, textures[0])); // Creates pucks
	}

	rink.set_texture(textures[1]); // Sets rink texture
	rink.load_rink_mesh_from_file("rink_outline.csv"); // Loads rink mesh from file
	if (rink.get_rink_mesh().size() == 0) {
        SDL_Log(ANSI_COLOR_RED "Failed to load rink mesh!" ANSI_COLOR_RESET);
    }
    else {
        SDL_Log(ANSI_COLOR_GREEN "Loaded rink mesh successfully!" ANSI_COLOR_RESET);
    }

	return SDL_APP_CONTINUE;  /* carry on with the program! */

}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{

    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
        break;
    case SDL_EVENT_KEY_DOWN:

        switch (event->key.scancode) {
        case SDL_SCANCODE_ESCAPE:
            return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
            break;
        case SDL_SCANCODE_RIGHT:
            buttons.right = true;
            break;
        case SDL_SCANCODE_LEFT:
            buttons.left = true;
            break;
        case SDL_SCANCODE_UP:
            buttons.up = true;
            break;
        case SDL_SCANCODE_DOWN:
            buttons.down = true;
            break;
        case SDL_SCANCODE_SPACE:
			pucks[0].set_vel_x(0.0f);
            pucks[0].set_vel_y(0.0f);
			break;
        default:
            break;
        }

        break;

    case SDL_EVENT_KEY_UP:

        switch (event->key.scancode) {
        case SDL_SCANCODE_RIGHT:
            buttons.right = false;
            break;
        case SDL_SCANCODE_LEFT:
            buttons.left = false;
            break;
        case SDL_SCANCODE_UP:
            buttons.up = false;
            break;
        case SDL_SCANCODE_DOWN:
            buttons.down = false;
            break;
        default:
            break;
        }

        break;

    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{

	frameStart = SDL_GetTicks();

    if (buttons.up) {
        pucks[0].set_vel_y(pucks[0].get_vel_y() - 0.1f);
    }
    if (buttons.down) {
        pucks[0].set_vel_y(pucks[0].get_vel_y() + 0.1f);
	}
    if (buttons.left) {
        pucks[0].set_vel_x(pucks[0].get_vel_x() - 0.1f);
	}
    if (buttons.right) {
        pucks[0].set_vel_x(pucks[0].get_vel_x() + 0.1f);
    }

    SDL_SetRenderDrawColorFloat(renderer, 0.3f, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);
    
	for (int i = 0; i < pucks.size(); i++) {
		pucks[i].update_position();
    }
    camera.adjust_cam_position(pucks[0].get_rel_x(), pucks[0].get_rel_y()); // center the camera on the puck

	// Draw rink using camera position
    SDL_FRect rink_rect = { (float)rink.get_screen_x(camera.get_x()), (float)rink.get_screen_y(camera.get_y()), (float)rink.get_width(), (float)rink.get_height()};
	SDL_RenderTexture(renderer, rink.get_texture(), NULL, &rink_rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  /* new color, full alpha. */
	for (int i = 0; i < rink.get_rink_mesh().size(); i++) {
        SDL_Point mesh_point = rink.get_rink_mesh_point(i);

        // Draw mesh nodes
        //SDL_FRect draw_point = { rink.get_screen_x(camera.get_x()) + mesh_point.x, rink.get_screen_y(camera.get_y()) + mesh_point.y, 4, 4 };
        //SDL_RenderFillRect(renderer, &draw_point);

        // Draw mesh lines
        SDL_RenderLine(renderer,
            rink.get_screen_x(camera.get_x()) + mesh_point.x,
            rink.get_screen_y(camera.get_y()) + mesh_point.y,
            rink.get_screen_x(camera.get_x()) + rink.get_rink_mesh_point((i + 1) % rink.get_rink_mesh().size()).x,
			rink.get_screen_y(camera.get_y()) + rink.get_rink_mesh_point((i + 1) % rink.get_rink_mesh().size()).y
		);

        for (int j = 0; j < num_pucks; j++) {

            if (rink.check_rink_mesh_collision(i, pucks[j].get_center_x(), pucks[j].get_center_y(), pucks[j].get_radius())) {
                printf("Collision detected at id: %d! Normal angle: %f\n", i, rink.get_normal(i));
                float norm_angle = rink.get_normal(i);

                float vel_x = pucks[j].get_vel_x();
                float vel_y = pucks[j].get_vel_y();
                float speed = sqrt(vel_x * vel_x + vel_y * vel_y);
                float current_angle = atan2f(vel_y, vel_x);

                float diff_angle = norm_angle - (current_angle - 3.14);

                //printf("Current_angle: %f, Diff angle: %f\n", current_angle, diff_angle);

                float reflected_angle = norm_angle + diff_angle;

                pucks[j].set_vel_x(speed * 1 * cos(reflected_angle));
                pucks[j].set_vel_y(speed * 1 * sin(reflected_angle));

                while (rink.check_rink_mesh_collision(i, pucks[j].get_center_x(), pucks[j].get_center_y(), pucks[j].get_radius())) {
                    pucks[j].set_rel_x(pucks[j].get_rel_x() + 1 * cos(norm_angle));
                    pucks[j].set_rel_y(pucks[j].get_rel_y() + 1 * sin(norm_angle));
                }

                if (num_pucks < 2) {
                    break;
                }

            }

        }

    }

	// Calculate puck screen pos and draw
    SDL_FRect puck_rect;
	for (int i = 0; i < pucks.size(); i++) {
        pucks[i].update_screen_position(rink.get_screen_x(camera.get_x()), rink.get_screen_y(camera.get_y()));
        puck_rect = pucks[i].get_rect();
        SDL_RenderTexture(renderer, pucks[i].get_texture(), NULL, &puck_rect);
    }

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    if (SDL_GetTicks() - frameStart < (1000 / TARGET_FPS)) {
        SDL_Delay((1000 / TARGET_FPS) - (SDL_GetTicks() - frameStart)); // Cap at ~75 FPS
	}

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{

	for (int i = 0; i < num_textures; i++) {
		if (textures[i] != NULL) {
			SDL_DestroyTexture(textures[i]);
		}
	}

    MIX_Quit();
	TTF_Quit();
    SDL_Quit();
}
