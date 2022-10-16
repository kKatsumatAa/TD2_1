#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "Util.h"
#include "Vector2.h"
#include "TextureManager.h"

class Particle
{
public:
	void Initialize(Vector3 startPos, Vector2 endPos, uint32_t r, uint32_t texture);
	void Update();
	void Draw();

	bool IsDead()const { return isDead_; }
private:
	Vector2 startPos_;
	Vector2 endPos_;
	uint32_t r_;
	Vector2 pos_;
	Sprite* sprite_;
	uint32_t texture_;
	float timer_ = 0;

	Vector2 p1_;

	bool isDead_ = false;
};

