#include "GravityObj.h"

void GravityObj::Initialize(Model* model, uint32_t* textureHandle, Gravity* gravity)
{
	assert(model);

	model_ = model;
	modelHand_ = model;
	textureHandle_ = textureHandle;

	this->gravity = gravity;

	isGravityObj = true;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	radius_ = 3.5f;

	velocity = { 0,0,0 };

	worldTransform_.Initialize();
	worldTransform_.scale_ = { radius_,radius_,1.0f };
	worldTransform_.translation_ = { -10,5,0 };

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void GravityObj::Update(GameSystem* gameSystem)
{
	if (gameSystem->GetIsStageChange())
	{
		worldTransform_.translation_ = { stageLeftTop.x + radius_, stageLeftTop.y + radius_,0 };

		switch (gameSystem->GetStage())
		{
		case 1:
			worldTransform_.translation_ = { stageLeftTop.x + radius_, stageLeftTop.y + radius_,0 };
			break;
		case 2:
			worldTransform_.translation_ = { stageLeftTop.x + radius_, stageLeftTop.y + radius_,0 };
			break;
		case 3:
			worldTransform_.translation_ = { stageLeftTop.x + radius_, stageLeftTop.y + radius_,0 };
			break;
		case 4:
			worldTransform_.translation_ = { stageLeftTop.x + radius_, stageLeftTop.y + radius_,0 };
			break;
		case 5:
			worldTransform_.translation_ = { stageLeftTop.x + radius_, stageLeftTop.y + radius_,0 };
			break;
		}
	}

	SetWorldPos(gravity->ObjMove(worldTransform_, 0.5f));
	worldTransform_.UpdateMatrix();
}

void GravityObj::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view);
}

Vector3 GravityObj::GetWorldPos()
{
	return worldTransform_.translation_;
}

void GravityObj::OnCollision(Collider& collider)
{
}

void GravityObj::OnCollision2(Collider& collider)
{
}
