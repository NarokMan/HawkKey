#include "Map.h"
#include "Rink.h"
#include <vector>
#include <string>
#include <iostream>
#include <SDL3/SDL.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

Map::Map(std::string map_name) {
	
	FILE* file;
	
	std::string cfg_name = "maps/";
	cfg_name = cfg_name + map_name + "/" + map_name + ".cfg";
	
	SDL_Log("Loading map %s...", map_name.c_str());
	SDL_Log("Finding cfg file %s...", cfg_name.c_str());
	
	file = fopen(cfg_name.c_str(), "r");
	if (!file) {
		SDL_Log(ANSI_COLOR_RED "Couldn't load cfg file: %s" ANSI_COLOR_RESET, SDL_GetError());
	}
	
	char line[256];
	int num_clusters;
	char cluster_files[1024][256]; // all the cluster csv files
	int collision[1024]; // all the collision types for the clusters
	
	int num_triggers;
	char trigger_files[1024][256]; // all the cluster csv files
	char temp_destination[256];
	std::vector <std::string> trigger_destinations; // all the destinations for the triggers
	
	int music_questionmark;
	char temp_music_file[256] = ""; // all the music files
	
	int player_x;
	int player_y;
	int player_angle;
	
	SDL_Log("Reading lines...");
	
	fgets(line, sizeof(line), file);
	SDL_Log(line);
	sscanf(line, "CLUSTERS %d", &num_clusters);
	SDL_Log("Number of clusters: %d", num_clusters);
	
	// Getting cluster file names and collision types
	for (int i = 0; i < num_clusters; i++) {
		
		fgets(line, sizeof(line), file);
		SDL_Log(line);
		sscanf(line, "%255s %d", cluster_files[i], &collision[i]);
		SDL_Log("Cluster %d: %s has collision type %d", i, cluster_files[i], collision[i]);
		
	}
	
	fgets(line, sizeof(line), file);
	
	fgets(line, sizeof(line), file);
	SDL_Log(line);
	sscanf(line, "TRIGGERS %d", &num_triggers);
	SDL_Log("Number of triggers: %d", num_triggers);
	
	// Getting cluster file names and collision types
	for (int i = 0; i < num_triggers; i++) {
		
		fgets(line, sizeof(line), file);
		SDL_Log(line);
		sscanf(line, "%255s %s", trigger_files[i], temp_destination);
		trigger_destinations.push_back(std::string(temp_destination));
		SDL_Log("Trigger %d: %s has destination %s", i, trigger_files[i], trigger_destinations[i].c_str());
		
	}
	
	fgets(line, sizeof(line), file);

	fgets(line, sizeof(line), file);
	SDL_Log(line);
	sscanf(line, "MUSIC %d", &music_questionmark);
	if (music_questionmark == 0)
		SDL_Log("There will be no music. :(");
	else {
		fgets(line, sizeof(line), file);
		sscanf(line, "%s", temp_music_file);
		SDL_Log("There will be music: %s", temp_music_file);
	}
	
	fgets(line, sizeof(line), file);
	fgets(line, sizeof(line), file);
	
	fgets(line, sizeof(line), file);
	SDL_Log(line);
	sscanf(line, "%d %d %d", &player_x, &player_y, &player_angle);
	SDL_Log("The player will start at %d, %d and be oriented %d degrees.", player_x, player_y, player_angle);
	
	fclose(file);
	
	SDL_Log("Making new map_data object...");
	num_clusters = num_clusters;
	num_triggers = num_triggers;
	player_start_x = player_x;
	player_start_y = player_y;
	player_start_angle = player_angle;
	
	SDL_Log("Grabbing the clusters...");
	collision_clusters = read_all_clusters(map_name, collision, num_clusters);
	trigger_clusters = read_all_triggers(map_name, trigger_destinations, num_triggers);
	
	music_file = "maps/";
	music_file = music_file + map_name + "/" + std::string(temp_music_file);
	
}

struct collision_cluster Map::read_cluster_file(std::string filename, enum collision_type collision) {
	
	FILE* file;
	
	SDL_Log("Getting file %s...", filename.c_str());
	file = fopen(filename.c_str(), "r");
	if (file == NULL) {
		SDL_Log(ANSI_COLOR_RED "Couldn't find file: %s" ANSI_COLOR_RESET, SDL_GetError());
	}
		
	struct collision_cluster new_cluster;
	new_cluster.collision = collision;
		
	char line[50];
	float num1;
	float num2;
	int current_node = 0;
	while (fgets(line, sizeof(line), file)) {
	
		sscanf(line, "%f, %f", &num1, &num2);
		new_cluster.node_array.push_back({num1, num2});
		
	}
	
	fclose(file);
	
	return new_cluster;
	
}

std::vector<struct collision_cluster> Map::read_all_clusters(std::string map_name, int collision[], int num_of_clusters) {
	
	struct collision_cluster temp_cluster;
	std::string cluster_filename;
	
	std::vector<struct collision_cluster> cluster_array;
	
	SDL_Log("Reading the cluster files...");
	for (int i = 1; i < num_of_clusters + 1; i++) {
		
		cluster_filename = "maps/";
		cluster_filename = cluster_filename + map_name + "/clusters/" + map_name + "_cluster_" + std::to_string(i) + ".csv";
		
		if (collision[i - 1] == 0) {
			temp_cluster = read_cluster_file(cluster_filename, NONE);
			SDL_Log("Cluster %d created with collision type NONE", i - 1);
		}
		else if (collision[i - 1] == 1) {
			temp_cluster = read_cluster_file(cluster_filename, INSIDE);
			SDL_Log("Cluster %d created with collision type INSIDE", i - 1);
		}
		else if (collision[i - 1] == 2) {
			temp_cluster = read_cluster_file(cluster_filename, OUTSIDE);
			SDL_Log("Cluster %d created with collision type OUTSIDE", i - 1);
		}
		
		cluster_array.push_back(temp_cluster);
		
	}
	
	return cluster_array;
	
}


struct trigger_cluster Map::read_trigger_file(std::string filename, std::string destination) {
	
	FILE* file;
	
	SDL_Log("Getting file %s...", filename.c_str());
	file = fopen(filename.c_str(), "r");
	if (file == NULL) {
		SDL_Log(ANSI_COLOR_RED "Couldn't find file: %s" ANSI_COLOR_RESET, SDL_GetError());
	}
		
	struct trigger_cluster new_trigger;
	new_trigger.destination_map_name = destination;
		
	char line[50];
	float num1;
	float num2;
	int current_node = 0;
	while (fgets(line, sizeof(line), file)) {
	
		sscanf(line, "%f, %f", &num1, &num2);
		new_trigger.node_array.push_back({num1, num2});
		
	}
	
	fclose(file);
	
	return new_trigger;	
	
}

std::vector<struct trigger_cluster> Map::read_all_triggers(std::string map_name, std::vector<std::string> destination, int num_of_clusters) {
	
	struct trigger_cluster temp_cluster;
	std::string cluster_filename;
	
	std::vector<struct trigger_cluster> trigger_array;
	
	SDL_Log("Reading the trigger files of the %d triggers...", num_of_clusters);
	for (int i = 1; i < num_of_clusters + 1; i++) {
		
		cluster_filename = "maps/";
		cluster_filename = cluster_filename + map_name + "/triggers/" + map_name + "_trigger_" + std::to_string(i) + ".csv";

		temp_cluster = read_trigger_file(cluster_filename, destination[i - 1]);

		trigger_array.push_back(temp_cluster);
		
	}
	
	return trigger_array;
	
}

bool Map::check_mesh_collision(int cluster_id, int node_id, int puck_x, int puck_y, int puck_radius)
{
    int id_1 = node_id;
	int id_2 = (node_id + 1) % collision_clusters[cluster_id].node_array.size();

	// Vector from point 1 to point 2
	float dx = collision_clusters[cluster_id].node_array[id_2].x - collision_clusters[cluster_id].node_array[id_1].x;
	float dy = collision_clusters[cluster_id].node_array[id_2].y - collision_clusters[cluster_id].node_array[id_1].y;

	// Vector from point 1 to puck center
	float fx = puck_x - collision_clusters[cluster_id].node_array[id_1].x;
	float fy = puck_y - collision_clusters[cluster_id].node_array[id_1].y;

    float t = (fx * dx + fy * dy) / (dx * dx + dy * dy);

    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;

	float closest_x = collision_clusters[cluster_id].node_array[id_1].x + t * dx;
	float closest_y = collision_clusters[cluster_id].node_array[id_1].y + t * dy;

	float dist_x = puck_x - closest_x;
	float dist_y = puck_y - closest_y;

	float distance_squared = dist_x * dist_x + dist_y * dist_y;

    return distance_squared <= puck_radius * puck_radius;
}

float Map::get_regular_func(float angle) {

    while (angle > 3.141592653f) {
		angle -= 2.0f * 3.141592653f;
    }

    while (angle < -3.141592653f) {
        angle += 2.0f * 3.141592653f;
    }

	return angle;

}

float Map::get_normal(int cluster_id, int node_id, enum collision_type collision) {

	int id_1, id_2;
	if (collision == INSIDE) {
		id_1 = node_id;
		id_2 = (node_id + 1) % collision_clusters[cluster_id].node_array.size();
	} else if (collision == OUTSIDE) {
		id_2 = node_id;
		id_1 = (node_id + 1) % collision_clusters[cluster_id].node_array.size();
	}

    // Vector from point 1 to point 2
    float dx = collision_clusters[cluster_id].node_array[id_2].x - collision_clusters[cluster_id].node_array[id_1].x;
    float dy = collision_clusters[cluster_id].node_array[id_2].y - collision_clusters[cluster_id].node_array[id_1].y;

	// Normal vector
    float normal_x = -dy;
    float normal_y = dx;

	float angle = atan2f(normal_y, normal_x); // Angle in radians
	angle = get_regular_func(angle);

	return angle;

}
