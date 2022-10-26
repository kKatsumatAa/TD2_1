#include "EffectParticle.h"

void Particle::Initialize(Vector3 startPos, Vector2 endPos, uint32_t r, uint32_t texture)
{
	//3D座標を2Dに変換
	startPos_.x = (startPos.x + 32) * 20;
	startPos_.y = (startPos.y - 18) * -20;

	endPos_.x = endPos.x;
	endPos_.y = endPos.y;

	r_ = r;

	//ベジエ曲線
	p1_ = Vector2(startPos_.x - 300,startPos_.y - 300);

	//テクスチャ
	texture_ = texture;

	sprite_ = Sprite::Create(texture_, { startPos_.x - r,startPos_.y - r });
}

void Particle::Update()
{
	Vector2 p3 = lerp(startPos_, p1_, timer_);
	Vector2 p4 = lerp(p1_, endPos_, timer_);

	pos_ = lerp(p3, p4, timer_);
	timer_ += 0.01f;
	if (timer_ >= 1) {
		isDead_ = true;
	}
	sprite_->SetPosition(Vector2(pos_.x - r_,pos_.y - r_));
}

void Particle::Draw()
{
	sprite_->Draw();
}
