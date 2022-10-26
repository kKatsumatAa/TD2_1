#include "Check.h"


void Check::Initialize(uint32_t texture, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle)
{
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9;j++) {
			sprite_[(i * 9) + j] = Sprite::Create(texture, Vector2(pos_.x + i * 80, pos_.y + j * 80));
			sprite_[(i * 9) + j]->SetTextureRect({ pos_.x + i * 80, pos_.y + j * 80 }, {80,80});
		}
	}
	for (int i = 0; i < 144; i++) {
		sprite_[i]->SetSize(size_);
	}

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;
}

void Check::Update()
{
	bool oldTime = timer_;

	if (size_.x < 80 && back_ == false) {
		pos_ -= speed_ / 2;
		size_ += speed_;
	}
	if (size_.x >= 80) {
		back_ = true;
		timer_--;
	}
	if (timer_ <= 0 && !(oldTime <= 0))
	{
		voiceHandle[5] = audio->PlayWave(soundDataHandle[5], false, 2.0f);
	}
	if (back_ == true && timer_ <= 0) {
		pos_ += speed_ / 2;
		size_ -= speed_;
		if (size_.x <= 0) {
			isDead_ = true;
		}
	}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {
			sprite_[(i * 9) + j]->SetPosition(Vector2(pos_.x + i * 80, pos_.y + j * 80));
		}
	}
	for (int i = 0; i < 144; i++) {
		sprite_[i]->SetSize(size_);
	}
}

void Check::Draw()
{
	for (int i = 0; i < 144; i++) {
		sprite_[i]->Draw();
	}
}
