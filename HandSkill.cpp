#include "HandSkill.h"

void HandSkill::Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.UpdateMatrix();

	//�V���O���g���C���X�^���X���擾
	debugText_ = DebugText::GetInstance();

	//�Փˑ���
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void HandSkill::Update()
{
	deathTimer_++;

	//x�������₵�đ�p
	radius_ = LerpVector3({ 0,0,0 }, { radiusMax,0,0 }, EaseOut((float)deathTimer_ / (float)lifeTime)).x;

	worldTransform_.scale_ = { radius_,radius_ ,radius_ };
	worldTransform_.UpdateMatrix();

	if (deathTimer_ >= lifeTime) isDead = true;
}

void HandSkill::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_[3]);
}

Vector3 HandSkill::GetWorldPos()
{
	return worldTransform_.translation_;
}

void HandSkill::OnCollision()
{
}

void HandSkill::OnCollision2(Collider& collider)
{
}