#pragma once

class Camera
{

public:
	Camera();
	void adjust_cam_position(int puck_x, int puck_y);

	int get_x();
	int get_y();
	void set_x(int new_x);
	void set_y(int new_y);

private:
	int x;
	int y;
	int dx;
	int dy;
};

