#include "SceneEffectManager.h"

void SceneEffectManager::Initialize(uint32_t* texture, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle)
{
	sceneTexture_ = texture[7];
	fishTexture_ = texture[5];
	gameovertexture_ = texture[8];
	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;
	texture_ = texture;
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
	newEffect->Initialize(sceneTexture_, audio, soundDataHandle, voiceHandle);
	nomal_.push_back(std::move(newEffect));

	voiceHandle[6] = audio->PlayWave(soundDataHandle[6], false, 2.0f);
}

void SceneEffectManager::SchoolOfFishGenerate()
{
	std::unique_ptr<SchoolOfFish> newFish = std::make_unique<SchoolOfFish>();
	newFish->Initialize(fishTexture_);
	fish_.push_back(std::move(newFish));
}

void SceneEffectManager::CheckGenerate(uint32_t texture)
{
	std::unique_ptr<Check> newFade = std::make_unique<Check>();
	newFade->Initialize(texture_[texture], audio, soundDataHandle, voiceHandle);
	fade_.push_back(std::move(newFade));
	voiceHandle[6] = audio->PlayWave(soundDataHandle[6], false, 2.0f);
}

