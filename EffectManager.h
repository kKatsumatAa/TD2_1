#pragma once
#include "Model.h"
#include "TextureManager.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "Model.h"
#include "Shake.h"

#include "EffectBurst.h"
#include "EffectParticle.h"

class EffectManager
{
public:
	void Initialize(uint32_t* texture);
	void Update();
	void Draw(ViewProjection viewProjection);
	void SpriteDraw();

	//��юU��G�t�F�N�g(���W,��)
	void BurstGenerate(Vector3 pos, uint32_t num, float range = 0.5f, float pow = 2.0f);
	//�p�[�e�B�N���G�t�F�N�g(���W,�摜�̔��a,�e�N�X�`���n���h��)
	void ParticleGenerate(Vector3 pos, Vector2 endPos);
	//��ʃV�F�C�N
	void ShakeGenerate(int time);
	Vector3 ShakePow();

private:
	ViewProjection viewProjection_;
	Model* model_ = nullptr;
	Shake shake_;

	std::list<std::unique_ptr<Burst>> burst_;
	std::list<std::unique_ptr<Particle>> particle_;

	uint32_t particleR_ = 16;
	uint32_t particleTexture_ = 0;
	uint64_t burstTexture_;
};