#pragma once
#include "Player.h"

class Item : public Collider
{
private:
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;
	HandStop* handStop = nullptr;
	EffectManager* effectManager = nullptr;

	GameSystem* gameSystem = nullptr;

	int bonusTime = 0;
	int timer = 0;

public:
	int useHandCount = 0;
	Input* input_ = nullptr;


	void Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos, HandStop* handStop, EffectManager* effectManager,
		GameSystem* gameSystem);
	void Update();
	void Draw(const ViewProjection& view);

	int GetBonusTime() { return bonusTime; }

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };

	float GetAngle() { return worldTransform_.rotation_.z; }

	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};

