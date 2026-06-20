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
	
	int num_clusters;
    std::vector<collision_cluster> collision_clusters;
    
    int num_triggers;
    std::vector<trigger_cluster> trigger_clusters;
    
    std::string music_file;
    
    int player_start_x;
    int player_start_y;
    int player_start_angle;
    
    Map(std::string map_name);
    
    bool check_mesh_collision(int cluster_id, int node_id, int puck_x, int puck_y, int puck_radius);
    bool check_trigger_collision(int trigger_id, int node_id, int puck_x, int puck_y, int puck_radius);
    float get_regular_func(float angle);
    float get_normal(int cluster_id, int id, enum collision_type collision);
    
    struct collision_cluster read_cluster_file(std::string filename, enum collision_type collision);
	std::vector<struct collision_cluster> read_all_clusters(std::string map_name, int collision[], int num_of_clusters);
	
	struct trigger_cluster read_trigger_file(std::string filename, std::string destination);
	std::vector<struct trigger_cluster> read_all_triggers(std::string map_name, std::vector<std::string> destination, int num_of_clusters);
	
};
