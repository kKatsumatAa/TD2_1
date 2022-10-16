#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "Vector2.h"

class SchoolOfFish
{
public:
	void Initialize(uint32_t texture);
	void Update();
	void Draw();

	bool IsDead() {
		return isDead_;
	}

private:
	Sprite* sprite_;
	bool isDead_ = false;

	Vector2 pos_;
};

