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

	//飛び散りエフェクト(座標,数)
	void BurstGenerate(Vector3 pos, uint32_t num);

private:
	ViewProjection viewProjection_;
	Model* model_ = nullptr;


	std::list<std::unique_ptr<Burst>> burst_;

};