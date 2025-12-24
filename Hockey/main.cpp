#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Puck.h"
#include "Rink.h"
#include <vector>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

 /* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

int num_textures = 2;
std::vector<SDL_Texture*> textures;

Rink rink(0, 0, 3000, 1275, NULL);
std::vector<Puck> pucks;

int frameStart = 0;

MIX_Mixer* mixer = NULL;

/*  This function runs once at startup. 
    Its parameters are:
    'appstate', which gives an optional space to store a pointer for future use
    'argc', which is just normal argc
    'argv' which is just normal argv */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{

    SDL_SetAppMetadata("SDL3 Window", "0.1", "com.narok.template"); // Sets app metadata

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

	pucks.push_back(Puck(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1.0f, 1.0f, 20, textures[0])); // Creates a puck
	rink.set_texture(textures[1]); // Sets rink texture

	return SDL_APP_CONTINUE;  /* carry on with the program! */

}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) { // User clicks the red x
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{

	frameStart = SDL_GetTicks();

    SDL_SetRenderDrawColorFloat(renderer, 0.3f, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

	// Draw rink
	SDL_FRect rink_rect = { 0.0f, 0.0f, (float)rink.get_width(), (float)rink.get_height() };
	SDL_RenderTexture(renderer, rink.get_texture(), NULL, &rink_rect);

	// Draw puck
	pucks[0].update_position();
	SDL_FRect puck_rect = pucks[0].get_rect();
	SDL_RenderTexture(renderer, textures[0], NULL, &puck_rect);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    if (SDL_GetTicks() - frameStart < 13) {
        SDL_Delay(13 - (SDL_GetTicks() - frameStart)); // Cap at ~75 FPS
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
