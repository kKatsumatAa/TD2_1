#pragma once
#include"Vector3.h"
#include"Vector2.h"
#include"CollisionConfig.h"
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"
#include "HandStop.h"
#include <cstdint>


/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider
{
private:
	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突属性(相手)
	uint32_t collisionMask_ = 0xffffffff;

	

protected:
	float radius_ = 1.0f;
	//死亡フラグ
	bool isDead = false;
	//掴まれている手の数（敵用）
	int handCount = 0;
	WorldTransform worldTransform_;

public:
	int GetHandCount() { return handCount; }
	void AddHandCount(int add) { handCount += add; }
	bool GetIsDead() { return isDead; }
	float GetRadius();
	void SetRadius(const float& radius);

	//衝突時に呼ばれる
	virtual void OnCollision() = 0;
	//手と敵の判定用
	virtual void OnCollision2(Collider& collider) = 0;
	//world座標をゲット
	virtual Vector3 GetWorldPos() = 0;
	void SetWorldPos(Vector3 pos) { worldTransform_.translation_ = pos; }

	//衝突用ビット系
	//自分
	uint32_t GetCollisionAttribute();
	void SetCollisionAttribute(const uint32_t& bit);
	//敵
	uint32_t GetCollisionMask();
	void SetCollisionMask(const uint32_t& bit);
};

