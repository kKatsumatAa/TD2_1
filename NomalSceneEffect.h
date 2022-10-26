#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "Vector2.h"
#include "Audio.h"

class NomalSceneEffect
{
public:
	void Initialize(uint32_t texture, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle);
	void Update();
	void Draw();

	bool IsDead() {
		return isDead_;
	};

private:
	Sprite* sprite_;
	bool isDead_ = false;
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;

	Vector2 pos_ = {Vector2( - 1280,0)};
	bool back_ = false;
	int timer_ = 100;
	int speed_ = 20;
};

