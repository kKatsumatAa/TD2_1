#include "SceneEffectManager.h"

void SceneEffectManager::Initialize()
{
	sceneTexture_ = TextureManager::Load("sceneEffect.png");
}

void SceneEffectManager::Update()
{
	nomal_.remove_if([](std::unique_ptr<NomalSceneEffect>& effect) {return effect->IsDead(); });
	for (std::unique_ptr<NomalSceneEffect>& effect : nomal_) {
		effect->Update();
	}
}

void SceneEffectManager::Draw()
{
	for (std::unique_ptr<NomalSceneEffect>& effect : nomal_) {
		effect->Draw();
	}
}

void SceneEffectManager::NormalSceneEffectGenerate()
{
	std::unique_ptr<NomalSceneEffect> newEffect = std::make_unique<NomalSceneEffect>();
	newEffect->Initialize(sceneTexture_);
	nomal_.push_back(std::move(newEffect));
}

