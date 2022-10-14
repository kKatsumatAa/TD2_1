#include "Gravity.h"

Vector3 Gravity::Move(Vector3 pos,float gavitySpeed, float playerSprrd)
{
	if (axis == Axis::DOWN) {
		pos.y -= gavitySpeed;
		if (pos.y < -18) {
			pos.y += gavitySpeed;
			if (playerWay == Way::CLOCK) {
				pos.x += playerSprrd;
				if (pos.x > 4) {
					pos.x -= playerSprrd;
					playerWay = Way::ANTICLOCK;
				}
			}
			else {
				pos.x -= playerSprrd;
				if (pos.x < -34) {
					pos.x -= playerSprrd;
					playerWay = Way::CLOCK;
				}
			}
		}
	}
	else if (axis == Axis::UP) {
		pos.y += gavitySpeed;
		if (pos.y > 18) {
			pos.y -= gavitySpeed;
			if (playerWay == Way::CLOCK) {
				pos.x -= playerSprrd;
				if (pos.x < -34) {
					pos.x += playerSprrd;
					playerWay = Way::ANTICLOCK;
				}
			}
			else {
				pos.x += playerSprrd;
				if (pos.x > 4) {
					pos.x -= playerSprrd;
					playerWay = Way::CLOCK;
				}
			}
		}
	}
	else if (axis == Axis::RIGHT) {
		pos.x += gavitySpeed;
		if (pos.x > 4) {
			pos.x -= gavitySpeed;
			if (playerWay == Way::CLOCK) {
				pos.y -= playerSprrd;
				if (pos.y < -18) {
					pos.y += playerSprrd;
					playerWay = Way::ANTICLOCK;
				}
			}
			else {
				pos.y += playerSprrd;
				if (pos.y > 18) {
					pos.y -= playerSprrd;
					playerWay = Way::CLOCK;
				}
			}
		}
	}
	else {
		pos.x -= gavitySpeed;
		if (pos.x < -33) {
			pos.x += gavitySpeed;
			if (playerWay == Way::CLOCK) {
				pos.y += playerSprrd;
				if (pos.y > 18) {
					pos.y -= playerSprrd;
					playerWay = Way::ANTICLOCK;
				}
			}
			else {
				pos.y -= playerSprrd;
				if (pos.y < -18) {
					pos.y += playerSprrd;
					playerWay = Way::CLOCK;
				}
			}
		}
	}

	return pos;
}