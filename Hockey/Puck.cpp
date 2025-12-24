#include "Puck.h"

Puck::Puck(int init_x, int init_y, float init_vel_x, float init_vel_y, int init_radius)
{
	x = init_x;
	y = init_y;
	vel_x = init_vel_x;
	vel_y = init_vel_y;
	radius = init_radius;
}

int Puck::get_x() { return x; }
int Puck::get_y() { return y; }
float Puck::get_vel_x() { return vel_x; }
float Puck::get_vel_y() { return vel_y; }
int Puck::get_radius() { return radius; }

void Puck::set_x(int new_x) { x = new_x; }
void Puck::set_y(int new_y) { y = new_y; }
void Puck::set_vel_x(float new_vel_x) { vel_x = new_vel_x; }
void Puck::set_vel_y(float new_vel_y) { vel_y = new_vel_y; }
void Puck::set_radius(float new_radius) { radius = new_radius; }
