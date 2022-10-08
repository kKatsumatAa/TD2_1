#pragma once
#include"Vector2.h"
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"


//��̍ő�̒���
static const float handLengthMax = 15.0f;

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
	DebugText* debugText_ = nullptr;

	//��̏I���_
	Vector3 endPos;
	//�p�x
	float angle;
	//�g�p�����ǂ���
	bool IsUse = false;
	//����ł��邩�ǂ���
	bool IsGrab = false;
	//���˂�����
	bool IsGo = false;
	//���ǂ��Ă��Ă��邩
	bool IsBack = false;

	//player�{�̂̏ꏊ
	Vector3 playerPos;

	//�X�e�[�g
	HandState* state = nullptr;


public:
	Vector3 velocity_;


public:
	void ChangeState(HandState* state);

	void Initialize(Model* model, const uint32_t textureHandle);
	void Update(const float& angle,const Vector3& playerPos);
	void Draw(const ViewProjection& viewProjection);

	Vector3 GetWorldPos();
	void    SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; }
	void SetAngle(const float& angle) { this->angle = angle; }

	bool GetIsUse() { return IsUse; }
	void SetIsUse(const bool& isUse) { IsUse = isUse; }
	bool GetIsGrab() { return IsGrab; }
	void SetIsGrab(const bool& isGrab) { IsGrab = isGrab; }
	bool GetIsGo() { return IsGo; }
	void SetIsGo(const bool& isGo) { IsGo = isGo; }
	bool GetIsBack() { return IsBack; }
	void SetIsBack(const bool& isBack) { IsBack = isBack; }
	
	Vector3 GetplayerPos() { return playerPos; }
	void SetplayerPos(const Vector3& playerPos) { this->playerPos = playerPos; }
	void SetEndPos(const Vector3& pos) { endPos = pos; }
	void ReachOut(const Vector3& pos, const float& angle);
};


class HandNormal : public HandState
{
private:

public:
	void Update() override;
};

//�肪�L�тĂ���Ƃ�
class HandReachOut : public HandState
{
private:

public:
	void Update() override;
};

//����ł���Ƃ�
class HandGrab : public HandState
{
private:


public:
	void Update() override;
};
