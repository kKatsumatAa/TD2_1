#pragma once
#include "Collider.h"
#include"Gravity.h"
#include"Wall.h"


class GravityObj :
    public Collider
{
private:
	WorldTransform worldTransformHand_;
	//モデル
	Model* model_ = nullptr;
	Model* modelHand_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;

	Vector3 velocity;

public:
	Input* input_ = nullptr;
	Gravity* gravity;

	void Initialize(Model* model, uint32_t* textureHandle, Gravity* gravity);
	void Update();
	void Draw(const ViewProjection& view);

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };
	float GetAngle() { return worldTransform_.rotation_.z; }

	void SetVelocity(Vector3 vec) { velocity = vec; }
	Vector3 GetVelocity() { return velocity; }
	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};

