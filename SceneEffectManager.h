#pragma once
#include "TextureManager.h"
#include "Sprite.h"
#include "NomalSceneEffect.h"

class SceneEffectManager
{
public:
	void Initialize();
	void Update();
	void Draw();

	void NormalSceneEffectGenerate();

private:
	uint32_t sceneTexture_ = 0;

	std::list<std::unique_ptr<NomalSceneEffect>> nomal_;
};