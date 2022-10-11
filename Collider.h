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
	//�͂܂�Ă����̐��i�G�p�j
	int handCount = 0;
	WorldTransform worldTransform_;

public:
	int GetHandCount() { return handCount; }
	void AddHandCount(int add) { handCount += add; }
	bool GetIsDead() { return isDead; }
	float GetRadius();
	void SetRadius(const float& radius);

	//�Փˎ��ɌĂ΂��
	virtual void OnCollision() = 0;
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

