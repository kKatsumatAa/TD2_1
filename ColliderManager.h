#pragma once
#include <list>
#include "Collider.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class ColliderManager
{
private:
	ViewProjection* view = nullptr;

	std::list<Collider*> colliders_;
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	/// <summary>
	/// ���C�̕��̓����蔻��(player�ƓG�̂�list�ɓo�^���Ďg��)
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair2(Collider* colliderA, Collider* colliderB);

public:
	bool isItemMode = true;


	void Initialize();

	//player�ƓG
	void CheckAllCollisions();
	//��ƓG�̓����蔻��
	void CheckAllCollisions2();

	//���X�g�ɓo�^
	void SetListCollider(Collider* collider);
	//���X�g���N���A
	void ClearList();
};

