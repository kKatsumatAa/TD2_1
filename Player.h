#pragma once
#include"PlayerHand.h"
#include"HandSkillManager.h"


class Player;

class PlayerHandState
{
protected:
	Player* player = nullptr;

public:
	void SetPlayer(Player* player);
	virtual void Update() = 0;
};


class Player : public Collider
{
private:
	WorldTransform worldTransformHand_;
	//���f��
	Model* model_ = nullptr;
	Model* modelHand_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;
	HandSkillManager* skillManager;

	//�p�x
	float angle = 0.0f;

	//��̏��
	PlayerHandState* state = nullptr;
	PlayerHand handR;
	PlayerHand handL;
	//�g�p���̎�����ɓ����p
	PlayerHand* useHands[2];

public:
	int useHandCount = 0;
	Input* input_ = nullptr;


	void ChangeState(PlayerHandState* state);

	void Initialize(Model* model, uint32_t* textureHandle, HandSkillManager* skillManager);
	void Update();
	void Draw(const ViewProjection& view);
	//���L�΂�
	void ReachOut();

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };
	PlayerHand* GetHandR() { return &handR; }
	PlayerHand* GetHandL() { return &handL; }
	PlayerHand** GetUseHands() { return useHands; }
	float GetAngle() { return worldTransform_.rotation_.z; }
	HandSkillManager* GetSkillManager() { return skillManager; }

	//�Փ˂����o������Ăяo���i�R�[���o�b�N�֐��j
	void OnCollision()override;
	void OnCollision2(Collider& collider)override;
};


//�������Ă��Ȃ�
class NoGrab : public PlayerHandState
{
private:

public:
	void Update() override;
};

//�Ў�łP������ł����ԁi�������͈͂��������j
class OneHandOneGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};

//������g���Ă���
class TwoHand : public PlayerHandState
{
private:

public:
	void Update() override;
};

//����łP������ł����ԁi�ːi�̒�����͈̔͂��������j
class TwoHandOneGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};

//����łQ������ł����ԁi��ɉ��΂��Ă����|�����玟�̂�ɓːi�j
class TwoHandTwoGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};
