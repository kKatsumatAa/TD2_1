#pragma once
#include "Sprite.h"

class Number
{
public:
	void Initialize();
	void Update();
	void Draw(Vector2 pos, Vector4 color, uint32_t num);

private:
	Sprite* sprite_[5][10];
	uint32_t numTexture_;
};

