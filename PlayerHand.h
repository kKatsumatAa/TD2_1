#pragma once
#include "Collider.h"
#include"Wall.h"

//��̍ő�̒���
static const float handLengthMax = 30.0f;
static const float handVelocityExtend = 0.5f;

class PlayerHand;

class HandState
{
protected:
	PlayerHand* hand = nullptr;

public:
	void SetHand(PlayerHand* playerHand);
	virtual void Update() = 0;
};

class PlayerHand : public Collider
{
private:
	Model* model_ = nullptr;
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;
	Wall* wall = nullptr;

	//��̏I���_
	Vector3 endPos;
	//�p�x
	float angle;
	//�g�p�����ǂ���
	bool IsUse = false;
	bool IsGrabOld = false;
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

	void Initialize(Model* model, uint32_t* textureHandle, Wall* wall);
	void Update(const float& angle,const Vector3& playerPos);
	void Draw(const ViewProjection& viewProjection);

	Vector3 GetWorldPos() override;
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

	bool GetTriggerIsGrab() { return (!IsGrab && IsGrabOld); }
	Wall* GetWall() { return wall; }
	
	Vector3 GetplayerPos() { return playerPos; }
	void SetplayerPos(const Vector3& playerPos) { this->playerPos = playerPos; }
	void SetEndPos(const Vector3& pos) { endPos = pos; }
	void ReachOut(const Vector3& pos, const float& angle);

	void ResetFlag();

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
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
