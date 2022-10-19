#pragma once
#include"Player.h"


class Enemy : public Collider
{
	//�ϐ�
private:
	//���L����
	Player* player_ = nullptr;

	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;
	EffectManager* effectManager = nullptr;

	int aliveNum = NULL;

public:
	void Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos, EffectManager* effectManager);
	void Update();
	void Draw(const ViewProjection& view);

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };

	int GetAliveNum() { return aliveNum; }
	void SetAliveNum(int aliveNum) { this->aliveNum = aliveNum; }

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision()override;
	//��Ƃ̓����蔻��p
	void OnCollision2(Collider& collider)override;
};

