#pragma once
#include"Player.h"


class Enemy : public Collider
{
	//変数
private:
	//自キャラ
	Player* player_ = nullptr;

	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;

public:
	void Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos);
	void Update();
	void Draw(const ViewProjection& view);

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };

	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision()override;
	//手との当たり判定用
	void OnCollision2(Collider& collider)override;
};

