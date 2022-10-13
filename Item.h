#pragma once
#include "Player.h"

class Item : public Collider
{
private:
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;
	HandStop* handStop = nullptr;
	EffectManager* effectManager = nullptr;

	int bonusTime = 0;
	int timer = 0;

public:
	int useHandCount = 0;
	Input* input_ = nullptr;


	void Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos, HandStop* handStop, EffectManager* effectManager);
	void Update();
	void Draw(const ViewProjection& view);

	int GetBonusTime() { return bonusTime; }

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };

	float GetAngle() { return worldTransform_.rotation_.z; }

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision()override;
	void OnCollision2(Collider& collider)override;
};

