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
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//��̍ő�̒���
	const float handLengthMax = 10.0f;
	//�p�x
	float angle = 0.0f;

	//��̏��
	PlayerHandState* state = nullptr;
	Vector2 handVecR;
	Vector2 handVecL;
	
	


public:
	void ChangeState(PlayerHandState* state);

	void Initialize(Model* model, const uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& view);
	//���L�΂�
	void ReachOut();

	Vector3 GetWorldPos();
};

//�Ў�łP������ł�����
class OneHandOneGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};

//����łP������ł�����
class TwoHandOneGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};

//����łQ������ł�����
class TwoHandTwoGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};
