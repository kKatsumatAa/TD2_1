#include "Item.h"


void Item::Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos, HandStop* handStop, EffectManager* effectManager,
	GameSystem* gameSystem)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	this->effectManager = effectManager;
	debugText_ = DebugText::GetInstance();

	this->gameSystem = gameSystem;

	this->handStop = handStop;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.UpdateMatrix();

	//衝突属性
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);
}

void Item::Update()
{
	timer++;

	if (timer % 180 == 0)
	{
		bonusTime++;
	}
}

void Item::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_[4]);

	debugText_->SetPos(10, 100);
	debugText_->Printf("bonusTime:%d", bonusTime);
}

Vector3 Item::GetWorldPos()
{
	return worldTransform_.translation_;
}

void Item::OnCollision(Collider& collider)
{
	isDead = true;

	//重力の箱が当たった時のみボーナス
	if (collider.GetIsGravityObj())
	{
		//ボーナスタイム追加
		gameSystem->SetBornusTime(gameSystem->GetBornusTime() + GetBonusTime());
	}
}

void Item::OnCollision2(Collider& collider)
{
	//手の位置を敵の位置にする
	collider.SetWorldPos(worldTransform_.translation_);
	//
	if (collider.GetHandCount() == 0)
	{
		//掴まれている数をカウント
		SetHandCount(GetHandCount() + 1);
		collider.SetHandCount(1);
	}
}
