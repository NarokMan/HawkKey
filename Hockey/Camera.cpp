#include "Camera.h"

Camera::Camera()
{
	x = 0;
	y = 0;
}

void Camera::adjust_cam_position(int puck_x, int puck_y)
{
	if (x != puck_x) {
		dx = puck_x - x;
	}
	else {
		dx = 0;
	}

	if (y != puck_y) {
		dy = puck_y - y;
	}
	else {
		dy = 0;
	}

	x += dx / 20;
	y += dy / 20;
}

int Camera::get_x() { return x; }
int Camera::get_y() { return y; }
void Camera::set_x(int new_x) { x = new_x; }
void Camera::set_y(int new_y) { y = new_y; }