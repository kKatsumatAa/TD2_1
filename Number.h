#pragma once
#include "Sprite.h"

class Number
{
public:
	void Initialize(Sprite* sprite);
	void Update();
	void Draw();

private:
	Sprite* sprite_;
	uint32_t numTexture_;
};

