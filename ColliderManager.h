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
	/// レイの方の当たり判定(playerと敵のみlistに登録して使う)
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair2(Collider* colliderA, Collider* colliderB);

public:
	bool isItemMode = true;


	void Initialize();

	//playerと敵
	void CheckAllCollisions();
	//手と敵の当たり判定
	void CheckAllCollisions2();

	//リストに登録
	void SetListCollider(Collider* collider);
	//リストをクリア
	void ClearList();
};

