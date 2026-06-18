#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include "Rink.h"

enum collision_type {
	
	NONE = 0,
	INSIDE = 1,
	OUTSIDE = 2
	
};

struct ui_button {

	SDL_FRect rect;

};

struct node {

    float x, y;

};

struct collision_cluster {

    std::vector<struct node> node_array;
    enum collision_type collision;    

};

struct trigger_cluster {

    std::vector<struct node> node_array;
    std::string destination_map_name;

};

class Map {

public:
	
	Rink rink;
	
	int num_clusters;
    std::vector<collision_cluster> collision_clusters;
    
    int num_triggers;
    std::vector<trigger_cluster> trigger_clusters;
    
    std::string music_file;
    
    int player_start_x;
    int player_start_y;
    int player_start_angle;
    
    Map(std::string map_name);
    
    struct collision_cluster read_cluster_file(std::string filename, enum collision_type collision);
	std::vector<struct collision_cluster> read_all_clusters(std::string map_name, int collision[], int num_of_clusters);
	
	struct trigger_cluster read_trigger_file(std::string filename, std::string destination);
	std::vector<struct trigger_cluster> read_all_triggers(std::string map_name, std::vector<std::string> destination, int num_of_clusters);
	
};
