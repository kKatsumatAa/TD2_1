#pragma once
#include "Collider.h"

class HandSkill : public Collider
{
private:
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;
	//����
	static const int32_t lifeTime = 30;
	int32_t deathTimer_ = 0;
	float radiusMax;

public:

	void Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos, const float& maxRadius);
	void Update();
	void Draw(const ViewProjection& view);

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};

