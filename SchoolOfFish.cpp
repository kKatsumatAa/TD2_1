#include "SchoolOfFish.h"
#include <random>

void SchoolOfFish::Initialize(uint32_t texture)
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> y(-50, 670);

	//ランダムで出した値を入れる
	pos_ = { -100, y(engine)};

	sprite_ = Sprite::Create(texture, pos_);
	sprite_->SetSize(Vector2(100, 100));
}

void SchoolOfFish::Update()
{
	pos_.x++;
	sprite_->SetPosition(pos_);
	if (pos_.x >= 1280) {
		isDead_ = true;
	}
}

void SchoolOfFish::Draw()
{
	sprite_->Draw();
}
