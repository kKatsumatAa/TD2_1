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

	Wall* wall = nullptr;
	

	Vector3 velocity;

	//�p�x
	float angle = 0.0f;

	//��̏��
	PlayerHandState* state = nullptr;
	PlayerHand handR;

	//�ђʍU�����ǂ���
	bool isRush = false;

public:
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
	float GetAngle() { return worldTransform_.rotation_.z; }
	HandSkillManager* GetSkillManager() { return skillManager; }

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

//�Ў�łP������ł�����
class OneHandOneGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};

//���ʂ̍U�����Ă�����
class OneHandAttack : public PlayerHandState
{
private:

public:
	void Update() override;
};

//�ђʍU���̏������
class OneHandRushGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};
//�ђʍU�����Ă�����
class OneHandRushAttack : public PlayerHandState
{
private:

public:
	void Update() override;
};
//�ђʍU�����Ă�����2
class OneHandRushAttack2 : public PlayerHandState
{
private:
	int timer = 0;
	const int maxTimer = 15;

public:
	void Update() override;
};