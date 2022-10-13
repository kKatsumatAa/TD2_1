#include "Gravity.h"

Vector3 Gravity::Move(Vector3 pos,float speed)
{
	if (axis == Axis::DOWN) {
		pos.y -= speed;
		if (pos.y < -18) {
			pos.y += speed;
		}
	}
	else if (axis == Axis::UP) {
		pos.y += speed;
		if (pos.y > 18) {
			pos.y -= speed;
		}
	}
	else if (axis == Axis::RIGHT) {
		pos.x += speed;
		if (pos.x > 4) {
			pos.x -= speed;
		}
	}
	else {
		pos.x -= speed;
		if (pos.x < -33) {
			pos.x += speed;
		}
	}

	return pos;
}