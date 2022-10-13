#include "Item.h"


void Item::Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos, HandStop* handStop)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	debugText_ = DebugText::GetInstance();

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

void Item::OnCollision()
{
	isDead = true;
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
