#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <time.h>
#include <vector>
#include "Puck.h"
#include "Rink.h"
#include "Camera.h"
#include "Player.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define TARGET_FPS 75

struct controls {

    bool up = false;
    bool down = false;
    bool left = false;
	bool right = false;
	bool mouse_left = false;
	bool mouse_right = false;
	int mouseX = 0;
	int mouseY = 0;

};

struct controls buttons = { false, false, false, false };

bool charging_shot = false;
SDL_Time charge_shot_begin = 0;

 /* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

int num_textures = 3;
std::vector<SDL_Texture*> textures;

Rink rink(0, 0, 3000, 1275, NULL);

int num_pucks = 10;
std::vector<Puck> pucks;

int num_players = 1;
std::vector<Player> players;

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

    temp_surface = SDL_LoadPNG("thingies/Eyes/placeholder_player.png");
    if (temp_surface == NULL) {
        SDL_Log(ANSI_COLOR_RED "Failed to load rink image: %s" ANSI_COLOR_RESET, SDL_GetError());
        textures[2] = NULL;
    }
    else {
        SDL_Log(ANSI_COLOR_GREEN "Loaded rink image successfully!" ANSI_COLOR_RESET);
        textures[2] = SDL_CreateTextureFromSurface(renderer, temp_surface);
        SDL_DestroySurface(temp_surface);
    }

    srand(time(NULL));

    for (int i = 0; i < num_pucks; i++) {
        pucks.push_back(Puck(rand() % 3000, rand() % 1000, rand() % 100 / 10 + 1, rand() % 100 / 10 + 1, 10, textures[0], nullptr)); // Creates pucks
	}

    for (int i = 0; i < num_players; i++) {
        players.push_back(Player(rand() % 3000, rand() % 1000, 40, 0, textures[2], nullptr));
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
        case SDL_SCANCODE_D:
            buttons.right = true;
            break;
        case SDL_SCANCODE_A:
            buttons.left = true;
            break;
        case SDL_SCANCODE_W:
            buttons.up = true;
            break;
        case SDL_SCANCODE_S:
            buttons.down = true;
            break;
        case SDL_SCANCODE_SPACE:
            players[0].set_vel_x(0.0f);
            players[0].set_vel_y(0.0f);
            break;
        default:
            break;
        }

        break;

    case SDL_EVENT_KEY_UP:

        switch (event->key.scancode) {
        case SDL_SCANCODE_D:
            buttons.right = false;
            break;
        case SDL_SCANCODE_A:
            buttons.left = false;
            break;
        case SDL_SCANCODE_W:
            buttons.up = false;
            break;
        case SDL_SCANCODE_S:
            buttons.down = false;
            break;
        default:
            break;
        }

        break;

    case SDL_EVENT_MOUSE_MOTION:
		buttons.mouseX = event->motion.x;
		buttons.mouseY = event->motion.y;

        break;

    
    case SDL_EVENT_MOUSE_BUTTON_DOWN:

        switch (event->button.button) {

        case SDL_BUTTON_LEFT:
            buttons.mouse_left = true;
			charging_shot = true;
			charge_shot_begin = SDL_GetTicks();
            break;
        case SDL_BUTTON_RIGHT:
            buttons.mouse_right = true;
            break;

        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:

        switch (event->button.button) {

        case SDL_BUTTON_LEFT:
            buttons.mouse_left = false;
			charging_shot = false;
            
            if (players[0].possessed_puck != nullptr) {

				float charge_coeff = (SDL_GetTicks() - charge_shot_begin) / 500.0f; // Charge coefficient based on how long the shot was charged, in seconds
                if (charge_coeff > 3.0f) {
                    charge_coeff = 3.0f; // Cap the charge coefficient at 3 seconds
				}

				players[0].possessed_puck->possessing_player = nullptr; // Drop the puck
				players[0].possessed_puck->set_vel_x(players[0].get_vel_x() + cos(players[0].get_screen_angle() * (3.14159f / 180.0f)) * charge_coeff * 10); // Shoot the puck in the direction the player is facing, with velocity based on charge time
				players[0].possessed_puck->set_vel_y(players[0].get_vel_y() + sin(players[0].get_screen_angle() * (3.14159f / 180.0f)) * charge_coeff * 10);
				players[0].possessed_puck = nullptr; // Player no longer possesses the puck
				players[0].set_player_state(NOT_POSSESSING_NOT_STABBING); // Set player state to not possessing

            }

			charge_shot_begin = 0;
            break;
        case SDL_BUTTON_RIGHT:
            buttons.mouse_right = false;
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
        players[0].set_vel_y(players[0].get_vel_y() - 0.1f);
    }
    if (buttons.down) {
        players[0].set_vel_y(players[0].get_vel_y() + 0.1f);
	}
    if (buttons.left) {
        players[0].set_vel_x(players[0].get_vel_x() - 0.1f);
	}
    if (buttons.right) {
        players[0].set_vel_x(players[0].get_vel_x() + 0.1f);
    }

    SDL_SetRenderDrawColorFloat(renderer, 0.3f, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);
    
    for (int i = 0; i < players.size(); i++) {
        players[i].update_position();
    }

	for (int i = 0; i < pucks.size(); i++) {
		pucks[i].update_position();
    }
    camera.adjust_cam_position(players[0].get_rel_x() + players[0].get_radius(), players[0].get_rel_y() + players[0].get_radius()); // center the camera on the puck

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
                //printf("Collision detected at id: %d! Normal angle: %f\n", i, rink.get_normal(i));
                float norm_angle = rink.get_normal(i);

                float vel_x = pucks[j].get_vel_x();
                float vel_y = pucks[j].get_vel_y();
                float speed = sqrt(vel_x * vel_x + vel_y * vel_y);
                
                float norm_x = cos(norm_angle);
                float norm_y = sin(norm_angle);
                float tang_x = -norm_y; // Negative reciprocal of normal, perpendicular
                float tang_y = norm_x;

                float vel_normal = vel_x * norm_x + vel_y * norm_y;
                float vel_tangent = vel_x * tang_x + vel_y * tang_y;

				vel_normal = -vel_normal * 0.6;
				vel_tangent = vel_tangent * 0.95;

                vel_x = vel_normal * norm_x + vel_tangent * tang_x;
                vel_y = vel_normal * norm_y + vel_tangent * tang_y;

                pucks[j].set_vel_x(vel_x);
                pucks[j].set_vel_y(vel_y);

                while (rink.check_rink_mesh_collision(i, pucks[j].get_center_x(), pucks[j].get_center_y(), pucks[j].get_radius())) {
                    pucks[j].set_rel_x(pucks[j].get_rel_x() + 1 * cos(norm_angle));
                    pucks[j].set_rel_y(pucks[j].get_rel_y() + 1 * sin(norm_angle));
                }

                if (pucks[j].possessing_player != nullptr) {

                    pucks[j].possessing_player->possessed_puck = nullptr; // Drop the puck
                    pucks[j].possessing_player->set_player_state(NOT_POSSESSING_NOT_STABBING); // Set player state to not possessing
                    pucks[j].possessing_player = nullptr; // Player no longer possesses the puck

                }

                if (num_pucks < 2) {
                    break;
                }

            }

        }

        for (int j = 0; j < num_players; j++) {

            if (rink.check_rink_mesh_collision(i, players[j].get_center_x(), players[j].get_center_y(), players[j].get_radius())) {

                float norm_angle = rink.get_normal(i);

                float vel_x = players[j].get_vel_x();
                float vel_y = players[j].get_vel_y();
                float speed = sqrt(vel_x * vel_x + vel_y * vel_y);

                float norm_x = cos(norm_angle);
                float norm_y = sin(norm_angle);
                float tang_x = -norm_y; // Negative reciprocal of normal, perpendicular
                float tang_y = norm_x;

                float vel_normal = vel_x * norm_x + vel_y * norm_y;
                float vel_tangent = vel_x * tang_x + vel_y * tang_y;

                vel_normal = -vel_normal * 0.05;
                vel_tangent = vel_tangent * 0.8;

                vel_x = vel_normal * norm_x + vel_tangent * tang_x;
                vel_y = vel_normal * norm_y + vel_tangent * tang_y;

                players[j].set_vel_x(vel_x);
                players[j].set_vel_y(vel_y);

                while (rink.check_rink_mesh_collision(i, players[j].get_center_x(), players[j].get_center_y(), players[j].get_radius())) {
                    players[j].set_rel_x(players[j].get_rel_x() + 1 * cos(norm_angle));
                    players[j].set_rel_y(players[j].get_rel_y() + 1 * sin(norm_angle));
                }

                if (players[j].possessed_puck != nullptr) {

                    players[j].possessed_puck->possessing_player = nullptr; // Drop the puck
                    players[j].possessed_puck = nullptr; // Player no longer possesses the puck
                    players[j].set_player_state(NOT_POSSESSING_NOT_STABBING); // Set player state to not possessing

                }

            }

        }

    }

    for (int i = 0; i < num_pucks; i++) {

        if (pucks[i].get_total_velocity() < 0.5) {
            pucks[i].set_vel_x(0);
            pucks[i].set_vel_y(0);
        }

        // Friction
        pucks[i].multiply_vel_x(0.995f);
        pucks[i].multiply_vel_y(0.995f);

    }

	// Calculate puck screen pos and draw
    SDL_FRect puck_rect;
	for (int i = 0; i < pucks.size(); i++) {
        pucks[i].update_screen_position(rink.get_screen_x(camera.get_x()), rink.get_screen_y(camera.get_y()));
        puck_rect = pucks[i].get_rect();
        SDL_RenderTexture(renderer, pucks[i].get_texture(), NULL, &puck_rect);
    }

    SDL_FRect player_rect;
    for (int i = 0; i < players.size(); i++) {
        players[i].update_screen_position(rink.get_screen_x(camera.get_x()), rink.get_screen_y(camera.get_y()));

		for (int j = 0; j < num_pucks; j++) {
            if (players[i].colliding_with_puck(pucks[j].get_center_x(), pucks[j].get_center_y(), pucks[j].get_radius())) {
                //printf("Player %d is colliding with puck %d!\n", i, j);

				float dx = pucks[j].get_center_x() - players[i].get_center_x();
				float dy = pucks[j].get_center_y() - players[i].get_center_y();
				float angle = atan2f(dy, dx);

				float distance = sqrt(dx * dx + dy * dy);
				float overlap = players[i].get_radius() + pucks[j].get_radius() - distance;

                pucks[j].set_rel_x(pucks[j].get_rel_x() + (overlap + 1) * cos(angle));
                pucks[j].set_rel_y(pucks[j].get_rel_y() + (overlap + 1) * sin(angle));

				pucks[j].set_vel_x(players[i].get_vel_x() + 1 * cos(angle));
                pucks[j].set_vel_y(players[i].get_vel_y() + 1 * sin(angle));

                if (pucks[j].possessing_player == nullptr && players[i].is_facing_puck(angle * 180 / 3.14, 45)) {

                    printf("Collision with unpossessed puck!\n");

                    // Determines course of action based on player_state: 
                    // POSSESSING = 0,
                    // NOT_POSSESSING_STABBING = 1,
                    // NOT_POSSESSING_NOT_STABBING = 2
                    switch (players[i].get_player_state()) {

                    case NOT_POSSESSING_NOT_STABBING:

                        pucks[j].possessing_player = &players[i];
						players[i].possessed_puck = &pucks[j];
                        players[i].set_player_state(POSSESSING);
                        break;

                    }

                }

            }

        }

        player_rect = players[i].get_rect();
        SDL_RenderTextureRotated(renderer, players[i].get_texture(), NULL, &player_rect, players[i].get_screen_angle(), NULL, SDL_FLIP_NONE);

    }

	// Player's target angle is always at the mouse
    players[0].set_target_angle(atan2f((float)(buttons.mouseY - (players[0].get_screen_y(rink.get_screen_y(camera.get_y())) + players[0].get_radius())),
		(float)(buttons.mouseX - (players[0].get_screen_x(rink.get_screen_x(camera.get_x())) + players[0].get_radius()))) * (180.0f / 3.14159f));

	players[0].update_stick_angle();

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
