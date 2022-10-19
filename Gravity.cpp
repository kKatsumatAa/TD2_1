#include "Gravity.h"

void Gravity::Initialize(Model* model)
{
	model_ = model;
	pos_.Initialize();
	pos_.scale_ = { 3.0f,3.0f,3.0f };
}

void Gravity::Update()
{
	pos_.translation_ = ObjMove(pos_, 0.3f);
	pos_.UpdateMatrix();
}

void Gravity::Draw(ViewProjection view)
{
	model_->Draw(pos_, view);
}

Vector3 Gravity::Move(Vector3 pos, float gavitySpeed, float playerSprrd)
{
	if (axis == Axis::DOWN) {
		pos.y -= gavitySpeed;
		if (pos.y < -18) {
			pos.y += gavitySpeed;
			if (playerWay == Way::CLOCK) {
				pos.x += playerSprrd;
				if (pos.x > 3) {
					pos.x -= playerSprrd;
					playerWay = Way::ANTICLOCK;
				}
			}
			else {
				pos.x -= playerSprrd;
				if (pos.x < -33) {
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
				pos.x += playerSprrd;
				if (pos.x > 3) {
					pos.x -= playerSprrd;
					playerWay = Way::ANTICLOCK;
				}
			}
			else {
				pos.x -= playerSprrd;
				if (pos.x < -33) {
					pos.x += playerSprrd;
					playerWay = Way::CLOCK;
				}
			}
		}
	}
	else if (axis == Axis::RIGHT) {
		pos.x += gavitySpeed;
		if (pos.x > 3) {
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

Vector3 Gravity::ObjMove(WorldTransform world, float gavitySpeed)
{
	if (sugitaIsGomi) {
		gavitySpeed *= 0.2f;
	}
	if (axis == Axis::DOWN) {
		world.translation_.y -= gavitySpeed;
		if (world.translation_.y < -18 + world.scale_.y + 2) {
			world.translation_.y += gavitySpeed;
		}
	}
	else if (axis == Axis::UP) {
		world.translation_.y += gavitySpeed;
		if (world.translation_.y > 18 - world.scale_.y - 2) {
			world.translation_.y -= gavitySpeed;
		}
	}
	else if (axis == Axis::RIGHT) {
		world.translation_.x += gavitySpeed;
		if (world.translation_.x > 3 - world.scale_.x - 2) {
			world.translation_.x -= gavitySpeed;
		}
	}
	else {
		world.translation_.x -= gavitySpeed;
		if (world.translation_.x < -33 + world.scale_.x + 2) {
			world.translation_.x += gavitySpeed;
		}
	}
	return world.translation_;
}

