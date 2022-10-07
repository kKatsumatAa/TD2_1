#include "Player.h"

void Player::ChangeState(PlayerHandState* state)
{
	delete this->state;
	this->state = state;
	state->SetPlayer(this);
}

//----------------------------------------------------------------------
void Player::Initialize(Model* model, const uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	modelHand_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransformHand_.Initialize();
	worldTransformHand_.scale_ = { 0.5f,0.5f,0.5f };
}

void Player::Update()
{
	//playerの回転
	worldTransform_.rotation_.z += ((int)input_->PushKey(DIK_LEFTARROW) - (int)input_->PushKey(DIK_RIGHTARROW)) * 0.05f;
	worldTransform_.UpdateMatrix();

	//手の届く範囲用
	worldTransformHand_.translation_.x = cosf(worldTransform_.rotation_.z) * handLengthMax;
	worldTransformHand_.translation_.y = sinf(worldTransform_.rotation_.z) * handLengthMax;
	worldTransformHand_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_);
	modelHand_->Draw(worldTransformHand_, view, textureHandle_);
}

void Player::ReachOut()
{
}

Vector3 Player::GetWorldPos()
{
	return worldTransform_.translation_;
}


//--------------------------------------------------------------------------
void PlayerHandState::SetPlayer(Player* player)
{
	this->player = player;
}

void OneHandOneGrab::Update()
{
}

void TwoHandOneGrab::Update()
{
}

void TwoHandTwoGrab::Update()
{
}
