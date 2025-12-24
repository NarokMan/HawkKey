#pragma once
class Puck
{

public:

	Puck(int start_x, int start_y, float start_vel_x, float start_vel_y, int start_radius);

	int get_x();
	int get_y();
	float get_vel_x();
	float get_vel_y();
	int get_radius();

	void set_x(int new_x);
	void set_y(int new_y);
	void set_vel_x(float new_vel_x);
	void set_vel_y(float new_vel_y);
	void set_radius(float new_radius);

private:
	int x;
	int y;
	float vel_x;
	float vel_y;
	int radius;

};

