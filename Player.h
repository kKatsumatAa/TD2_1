#pragma once
#include"PlayerHand.h"


class Player;

class PlayerHandState
{
protected:
	Player* player = nullptr;

public:
	void SetPlayer(Player* player);
	virtual void Update() = 0;
};


class Player
{
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	WorldTransform worldTransformHand_;
	//���f��
	Model* model_ = nullptr;
	Model* modelHand_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	DebugText* debugText_ = nullptr;

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

	void Initialize(Model* model, const uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& view);
	//���L�΂�
	void ReachOut();

	Vector3 GetWorldPos();
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };
	PlayerHand* GetHandR() { return &handR; }
	PlayerHand* GetHandL() { return &handL; }
	PlayerHand** GetUseHands() { return useHands; }
	float GetAngle() { return worldTransform_.rotation_.z; }
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
