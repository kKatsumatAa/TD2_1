#pragma once
#include"Vector2.h"
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"


class PlayerHand;

class HandState
{
protected:
	PlayerHand* hand = nullptr;

public:
	void SetHand(PlayerHand* playerHand);
	virtual void Update() = 0;
};

class PlayerHand
{
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//��̏I���_
	Vector3 endPos;
	//�p�x
	float angle;
	//�g�p�����ǂ���
	bool IsUse = false;

	//�X�e�[�g
	HandState* state = nullptr;


public:
	Vector3 velocity_;

public:
	void Initialize(Model* model, const uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	Vector3 GetWorldPos();
	void    SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; }
	void SetAngle(const float& angle) { this->angle = angle; }
	bool GetIsUse() { return IsUse; }
};

