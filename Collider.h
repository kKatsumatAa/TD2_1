#pragma once
#include"Vector3.h"
#include"Vector2.h"
#include"CollisionConfig.h"
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"DebugText.h"
#include"Util.h"
#include "Assert.h"
#include "WinApp.h"
#include "HandStop.h"
#include <cstdint>
#include <random>
#include"EffectManager.h"
#include"GameSystem.h"

//��
	//�����V�[�h������
static std::random_device seed_gen;
//�����Z���k�c�C�X�^�[
static std::mt19937_64 engine(seed_gen());
//�����͈�
static std::uniform_real_distribution<float> posDistX(-30.0f, -2.0f);
static std::uniform_real_distribution<float> posDistY(-13.0f, 13.0f);


/// <summary>
/// �Փ˔���I�u�W�F�N�g
/// </summary>
class Collider
{
private:
	//�Փˑ���(����)
	uint32_t collisionAttribute_ = 0xffffffff;
	//�Փˑ���(����)
	uint32_t collisionMask_ = 0xffffffff;

	

protected:
	float radius_ = 1.0f;
	//���S�t���O
	bool isDead = false;
	bool IsGrab = false;
	//�͂܂�Ă����̐��i�G�p�j
	int handCount = 0;
	WorldTransform worldTransform_;

	//�v���[���[���ǂ����i���莞�Ɏg���j
	bool isPlayer = false;
	//
	bool isGravityObj = false;
	//
	bool isItem = false;

	bool isTarget = false;

public:
	int GetHandCount() { return handCount; }
	void SetHandCount(const int& count) { handCount = count; }
	bool GetIsDead() { return isDead; }
	bool GetIsGrab() { return IsGrab; }
	void SetIsTarget(bool is) { isTarget = is; }
	bool GetIsTarget() { return isTarget; }
	bool GetIsPlayer() { return isPlayer; }
	bool GetIsItem() { return isItem; }
	bool GetIsGravityObj() { return isGravityObj; }
	float GetRadius();
	void SetRadius(const float& radius);
	void SetAngle(const float& angle) { worldTransform_.rotation_.z = angle; worldTransform_.UpdateMatrix(); }

	//�Փˎ��ɌĂ΂��
	virtual void OnCollision(Collider& collider) = 0;
	//��ƓG�̔���p
	virtual void OnCollision2(Collider& collider) = 0;
	//world���W���Q�b�g
	virtual Vector3 GetWorldPos() = 0;
	void SetWorldPos(Vector3 pos) { worldTransform_.translation_ = pos; }

	//�Փ˗p�r�b�g�n
	//����
	uint32_t GetCollisionAttribute();
	void SetCollisionAttribute(const uint32_t& bit);
	//�G
	uint32_t GetCollisionMask();
	void SetCollisionMask(const uint32_t& bit);
};

