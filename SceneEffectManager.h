#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "NomalSceneEffect.h"
#include "SchoolOfFish.h"
#include "Check.h"

class SceneEffectManager
{
public:
	void Initialize(uint32_t* texture);
	void Update();
	void Draw();

	void NormalSceneEffectGenerate();
	void SchoolOfFishGenerate();
	void CheckGenerate();

private:
	uint32_t sceneTexture_ = 0;
	uint32_t fishTexture_ = 0;
	uint32_t gameovertexture_ = 0;

	std::list<std::unique_ptr<NomalSceneEffect>> nomal_;
	std::list<std::unique_ptr<SchoolOfFish>> fish_;
	std::list<std::unique_ptr<Check>> fade_;
};