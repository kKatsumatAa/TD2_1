#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.UpdateMatrix();

	//衝突属性
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void Enemy::Update()
{

}

void Enemy::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view,textureHandle_[2]);
}

Vector3 Enemy::GetWorldPos()
{
	return worldTransform_.translation_;
}

void Enemy::OnCollision()
{
	isDead = true;
}

void Enemy::OnCollision2(Collider& collider)
{
	//手の位置を敵の位置にする
	collider.SetWorldPos(worldTransform_.translation_);
	//
	if (collider.GetHandCount() == 0)
	{
		//掴まれている数をカウント
		AddHandCount(1);
		collider.AddHandCount(1);
	}
}