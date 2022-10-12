#pragma once
#include "Model.h"
#include "TextureManager.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "Model.h"

#include "EffectBurst.h"

class EffectManager
{
public:
	void Initialize(ViewProjection viewProjection);
	void Update();
	void Draw();

	//��юU��G�t�F�N�g(���W,��)
	void BurstGenerate(Vector3 pos, uint32_t num);

private:
	ViewProjection viewProjection_;
	Model* model_ = nullptr;


	std::list<std::unique_ptr<Burst>> burst_;

};