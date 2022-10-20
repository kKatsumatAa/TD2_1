#include "SceneEffectManager.h"

void SceneEffectManager::Initialize()
{
	sceneTexture_ = TextureManager::Load("nextStage.png");
	fishTexture_ = TextureManager::Load("particle.png");
	gameovertexture_ = TextureManager::Load("gameover.png");
}

void SceneEffectManager::Update()
{
	nomal_.remove_if([](std::unique_ptr<NomalSceneEffect>& effect) {return effect->IsDead(); });
	for (std::unique_ptr<NomalSceneEffect>& effect : nomal_) {
		effect->Update();
	}
	fish_.remove_if([](std::unique_ptr<SchoolOfFish>& fish) {return fish->IsDead(); });
	for (std::unique_ptr<SchoolOfFish>& fish : fish_) {
		fish->Update();
	}
	fade_.remove_if([](std::unique_ptr<Check>& fade) {return fade->IsDead(); });
	for (std::unique_ptr<Check>& fade : fade_) {
		fade->Update();
	}
}

void SceneEffectManager::Draw()
{
	for (std::unique_ptr<NomalSceneEffect>& effect : nomal_) {
		effect->Draw();
	}
	for (std::unique_ptr<SchoolOfFish>& fish : fish_) {
		fish->Draw();
	}
	for (std::unique_ptr<Check>& fade : fade_) {
		fade->Draw();
	}
}

void SceneEffectManager::NormalSceneEffectGenerate()
{
	std::unique_ptr<NomalSceneEffect> newEffect = std::make_unique<NomalSceneEffect>();
	newEffect->Initialize(sceneTexture_);
	nomal_.push_back(std::move(newEffect));
}

void SceneEffectManager::SchoolOfFishGenerate()
{
	std::unique_ptr<SchoolOfFish> newFish = std::make_unique<SchoolOfFish>();
	newFish->Initialize(fishTexture_);
	fish_.push_back(std::move(newFish));
}

void SceneEffectManager::CheckGenerate()
{
	std::unique_ptr<Check> newFade = std::make_unique<Check>();
	newFade->Initialize(gameovertexture_);
	fade_.push_back(std::move(newFade));
}

