#pragma once
#include"PlayerHand.h"
#include"HandSkillManager.h"
#include"Gravity.h"


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
	HandStop* handStop;
	Wall* wall = nullptr;
	

	Vector3 velocity;

	//�p�x
	float angle = 0.0f;

	//��̏��
	PlayerHandState* state = nullptr;
	PlayerHand handR;
	PlayerHand handL;
	//�g�p���̎�����ɓ����p
	PlayerHand* useHands[2];

	//��̂̓G���̎�ł���ł��邩
	bool isTwoHandOneGrab = false;

public:
	int useHandCount = 0;
	Input* input_ = nullptr;
	Gravity* gravity;

	void ChangeState(PlayerHandState* state);

	void Initialize(Model* model, uint32_t* textureHandle, HandSkillManager* skillManager, HandStop* handStop, Wall* wall, Gravity* gravity);
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
	HandStop* GetHandStop() { return handStop; }

	void SetIsTwoHandOneGrab(const bool& is) { isTwoHandOneGrab = is; }
	bool GetIsTwoHandOneGrab() { return isTwoHandOneGrab; }

	void SetVelocity(Vector3 vec) { velocity = vec; }
	Vector3 GetVelocity() { return velocity; }
	Wall* GetWall() {return wall;}
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
class TwoHandOneGrab2 : public PlayerHandState
{
private:
	int timer = 0;
	const int maxTimer = 15;

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
