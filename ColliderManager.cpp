#include "ColliderManager.h"
#include "Util.h"

void ColliderManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask())
		|| !(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())
		|| (colliderA->GetIsDead() || colliderB->GetIsDead()))
	{
		return;//����A�Փˏ�������������
	}
	//�A�C�e���̃v���g�؂�ւ��ɑΉ����邽��
	if ((colliderA->GetIsPlayer() && colliderB->GetIsItem() && isItemMode)
		|| (colliderB->GetIsPlayer() && colliderA->GetIsItem() && isItemMode))
	{
		return;
	}

	Vector3 posA = colliderA->GetWorldPos();
	Vector3 posB = colliderB->GetWorldPos();

	float rA = colliderA->GetRadius();
	float rB = colliderB->GetRadius();

	if (CollisionCircleCircle(posA, rA, posB, rB))
	{
		colliderA->OnCollision(*colliderB);
		colliderB->OnCollision(*colliderA);
	}
}


void ColliderManager::CheckCollisionPair2(Collider* colliderA, Collider* colliderB)
{
	if ((colliderA->GetIsDead() || colliderB->GetIsDead()) || colliderA->GetIsGrab())
	{
		return;//����A�Փˏ�������������
	}

	Vector3 posA = colliderA->GetWorldPos();
	Vector3 posB = colliderB->GetWorldPos();

	float rA = colliderA->GetRadius();
	float rB = colliderB->GetRadius();

	if (CollisionCircleCircle(posA, rA, posB, rB))
	{
		if (colliderA->GetCollisionAttribute() == kCollisionAttributeHand && colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy)
		{
			colliderB->OnCollision2(*colliderA);
			colliderA->OnCollision(*colliderB);
		}
	}
}

void ColliderManager::Initialize()
{
	colliders_.clear();
	//view = nullptr;
	//projection = nullptr;
}

void ColliderManager::CheckAllCollisions()
{
	//���X�g���̃y�A�𑍓�����
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//itrB��itrA�̎��̗v�f����񂷁i�d�����������j
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			CheckCollisionPair(*itrA, *itrB);
		}
	}
}

void ColliderManager::CheckAllCollisions2()
{
	//���X�g���̃y�A�𑍓�����
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//itrB��itrA�̎��̗v�f����񂷁i�d�����������j
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			CheckCollisionPair2(*itrA, *itrB);
		}
	}
}

void ColliderManager::SetListCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void ColliderManager::ClearList()
{
	colliders_.clear();
}
