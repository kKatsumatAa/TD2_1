#pragma once
#include "Player.h"
#include "Number.h"
#include "Convert2D.h"
#include "Model.h"

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

	GameSystem* gameSystem = nullptr;

	int bonusTime = 0;
	Number* bonusTime_ = nullptr;
	int timer = 0;

	int aliveNum = NULL;

public:
	int useHandCount = 0;
	Input* input_ = nullptr;


	void Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos, HandStop* handStop, EffectManager* effectManager,
		GameSystem* gameSystem);
	void Update();
	void Draw(const ViewProjection& view);
	void DrawSprite();

	int GetBonusTime() { return bonusTime; }

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };

	float GetAngle() { return worldTransform_.rotation_.z; }

	int GetAliveNum() { return aliveNum; }
	void SetAliveNum(int aliveNum) { this->aliveNum = aliveNum; }

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};

