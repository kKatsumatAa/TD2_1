#include "EffectManager.h"

void EffectManager::Initialize()
{
	model_ = Model::Create();
	particleTexture_ = TextureManager::Load("particle.png");
}

void EffectManager::Update()
{
	//飛び散りエフェクト
	burst_.remove_if([](std::unique_ptr<Burst>& burst) {return burst->IsDead(); });
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Update();
	}
	//パーティクルエフェクト
	particle_.remove_if([](std::unique_ptr<Particle>& particle) {return particle->IsDead(); });
	for (std::unique_ptr<Particle>& particle : particle_) {
		particle->Update();
	}
}

void EffectManager::Draw(ViewProjection viewProjection)
{
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Draw(viewProjection);
	}
	
}

void EffectManager::SpriteDraw()
{
	for (std::unique_ptr<Particle>& particle : particle_) {
		particle->Draw();
	}
}

//飛び散りエフェクト
void EffectManager::BurstGenerate(Vector3 pos, uint32_t num, float range, float pow)
{
	for (int i = 0; i < num; i++) {
		std::unique_ptr<Burst> newBurst = std::make_unique<Burst>();
		newBurst->Initialize(model_, NULL, pos, range, pow);
		burst_.push_back(std::move(newBurst));
	}
}

void EffectManager::ParticleGenerate(Vector3 pos,Vector2 endPos)
{
	std::unique_ptr<Particle> newParticle = std::make_unique<Particle>();
	newParticle->Initialize(pos,endPos,particleR_,particleTexture_);
	particle_.push_back(std::move(newParticle));
}

void EffectManager::ShakeGenerate(int time)
{
	shake_.SetTimer(time);
}

Vector3 EffectManager::ShakePow()
{
	return shake_.ShakePow();
}
