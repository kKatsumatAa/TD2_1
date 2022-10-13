#include "EffectManager.h"

void EffectManager::Initialize(ViewProjection viewProjection)
{
	viewProjection_ = viewProjection;
	model_ = Model::Create();
}

void EffectManager::Update()
{
	//飛び散りエフェクト
	burst_.remove_if([](std::unique_ptr<Burst>& burst) {return burst->IsDead(); });
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Update();
	}
}

void EffectManager::Draw()
{
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Draw(viewProjection_);
	}
}

//飛び散りエフェクト
void EffectManager::BurstGenerate(Vector3 pos, uint32_t num, float range = 0.5f, float pow = 2.0f)
{
	for (int i = 0; i < num; i++) {
		std::unique_ptr<Burst> newBurst = std::make_unique<Burst>();
		newBurst->Initialize(model_, NULL, pos, range, pow);
		burst_.push_back(std::move(newBurst));
	}
}