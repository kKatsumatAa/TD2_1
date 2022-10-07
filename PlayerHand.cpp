#include "PlayerHand.h"

void PlayerHand::Initialize(Model* model, const uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void PlayerHand::Update()
{
	worldTransform_.UpdateMatrix();
}

void PlayerHand::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 PlayerHand::GetWorldPos()
{
	return worldTransform_.translation_;
}


//---------------------------------------------------------------
void HandState::SetHand(PlayerHand* playerHand)
{
	this->hand = playerHand;
}
