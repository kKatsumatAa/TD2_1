#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "NomalSceneEffect.h"
#include "SchoolOfFish.h"
#include "Check.h"


class SceneEffectManager
{
public:
	void Initialize(uint32_t* texture, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle);
	void Update();
	void Draw();

	void NormalSceneEffectGenerate(uint32_t texture);
	void SchoolOfFishGenerate();
	void CheckGenerate(uint32_t texture);

private:
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
	uint32_t sceneTexture_ = 0;
	uint32_t fishTexture_ = 0;
	uint32_t gameovertexture_ = 0;
	uint32_t* texture_;

	std::list<std::unique_ptr<NomalSceneEffect>> nomal_;
	std::list<std::unique_ptr<SchoolOfFish>> fish_;
	std::list<std::unique_ptr<Check>> fade_;
};