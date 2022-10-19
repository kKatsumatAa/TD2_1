#include "GravityObj.h"

void GravityObj::Initialize(Model* model, uint32_t* textureHandle, Gravity* gravity)
{
	assert(model);

	model_ = model;
	modelHand_ = model;
	textureHandle_ = textureHandle;

	this->gravity = gravity;


	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	radius_ = 2.0f;

	worldTransform_.Initialize();
	worldTransform_.scale_ = { radius_,radius_,radius_ };
	worldTransform_.translation_ = { -10,5,0 };

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void GravityObj::Update()
{
	SetWorldPos(gravity->ObjMove(worldTransform_, 0.3f));
	worldTransform_.UpdateMatrix();
}

void GravityObj::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_[0]);
}

Vector3 GravityObj::GetWorldPos()
{
	return worldTransform_.translation_;
}

void GravityObj::OnCollision()
{
}

void GravityObj::OnCollision2(Collider& collider)
{
}
