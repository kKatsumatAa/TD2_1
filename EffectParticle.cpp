#include "EffectParticle.h"

void Particle::Initialize(Vector3 startPos, Vector2 endPos, uint32_t r, uint32_t texture)
{
	//3D座標を2Dに変換
	startPos_.x = (startPos.x + 32) * 20;
	startPos_.y = (startPos.y - 18) * -20;

	endPos_.x = endPos.x;
	endPos_.y = endPos.y;

	//テクスチャ
	texture_ = texture;

	sprite_ = Sprite::Create(texture_, { startPos_.x - r,startPos_.y - r });
}

void Particle::Update()
{
	Vector2 length = endPos_ - startPos_;
	startPos_ += length.GetNormalize() * 5;
	sprite_->SetPosition(startPos_);

	if (length.GetLength() < 5) {
		isDead_ = true;
	}
}

void Particle::Draw()
{
	sprite_->Draw();
}
