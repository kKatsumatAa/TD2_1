#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "NomalSceneEffect.h"
#include "SchoolOfFish.h"

class SceneEffectManager
{
public:
	void Initialize();
	void Update();
	void Draw();

	void NormalSceneEffectGenerate();
	void SchoolOfFishGenerate();

private:
	uint32_t sceneTexture_ = 0;
	uint32_t fishTexture_ = 0;

	std::list<std::unique_ptr<NomalSceneEffect>> nomal_;
	std::list<std::unique_ptr<SchoolOfFish>> fish_;
};