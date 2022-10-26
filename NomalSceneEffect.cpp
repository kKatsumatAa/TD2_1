#include "NomalSceneEffect.h"


void NomalSceneEffect::Initialize(uint32_t texture, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle)
{
	sprite_ = Sprite::Create(texture, pos_);
	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;
}

void NomalSceneEffect::Update()
{
	bool oldTime = timer_;

	if (pos_.x != 0 && back_ == false) {
		pos_.x += speed_;
	}
	if (pos_.x == 0) {
		back_ = true;
		timer_--;
	}
	if (timer_ <= 0 && !(oldTime <= 0))
	{
 		voiceHandle[5] = audio->PlayWave(soundDataHandle[5], false, 2.0f);
	}
	if (back_ == true && timer_ <= 0) {
		pos_.x -= speed_;
		if (pos_.x == -1280) {
			isDead_ = true;
		}
	}
	sprite_->SetPosition(pos_);
}

void NomalSceneEffect::Draw()
{
	sprite_->Draw();
}
