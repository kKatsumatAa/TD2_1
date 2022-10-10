#pragma once
#include "Collider.h"

class HandSkill : public Collider
{
private:
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;
	//寿命
	static const int32_t lifeTime = 30;
	int32_t deathTimer_ = 0;
	const float radiusMax = 6.0f;

public:

	void Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos);
	void Update();
	void Draw(const ViewProjection& view);

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };

	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision()override;
	void OnCollision2(Collider& collider)override;
};

