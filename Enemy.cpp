#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos, EffectManager* effectManager)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	debugText_ = DebugText::GetInstance();

	this->effectManager = effectManager;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_.x = pi / -2.0f;
	worldTransform_.UpdateMatrix();

	//�Փˑ���
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void Enemy::Update()
{

}

void Enemy::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view);
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
	//��̈ʒu��G�̈ʒu�ɂ���
	collider.SetWorldPos(worldTransform_.translation_);
	//
	if (collider.GetHandCount() == 0)
	{
		//�͂܂�Ă��鐔���J�E���g
		SetHandCount(GetHandCount() + 1);
		collider.SetHandCount(1);
	}
}