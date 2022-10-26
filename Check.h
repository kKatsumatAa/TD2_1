#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "Vector2.h"
#include "Audio.h"

class Check
{
public:
	void Initialize(uint32_t texture, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle);
	void Update();
	void Draw();

	bool IsDead() {
		return isDead_;
	};

private:
	Sprite* sprite_[144];
	bool isDead_ = false;

	Vector2 pos_ = { Vector2(40,40) };
	Vector2 size_ = { Vector2(0,0) };
	bool back_ = false;
	int timer_ = 100;
	Vector2 speed_ = { Vector2(1,1) };
	int alpha_ = 0;

	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
};

