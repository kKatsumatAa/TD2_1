#include "PlayerHand.h"

void PlayerHand::Initialize(Model* model, uint32_t* textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	//最初は何もしていない
	state = new HandNormal;
	state->SetHand(this);

	//radius_ = 2.0f;

	//衝突属性
	SetCollisionAttribute(kCollisionAttributeHand);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void PlayerHand::Update(const float& angle, const Vector3& playerPos)
{
	//伸ばしていないときに回転、playerの位置に置く
	if (!IsUse) {
		worldTransform_.translation_ = playerPos;
		this->worldTransform_.rotation_.z = angle;
	}

	this->playerPos = playerPos;

	state->Update();

	worldTransform_.UpdateMatrix();
}

void PlayerHand::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection,textureHandle_[1]);
	debugText_->SetPos(0, 0);
	debugText_->Printf("%f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	debugText_->SetPos(0, 10);
	debugText_->Printf("isuse:%d", IsUse);
	debugText_->SetPos(0, 25);
	debugText_->Printf("isback:%d", IsBack);
	debugText_->SetPos(0, 40);
	debugText_->Printf("isGo:%d", IsGo);
	debugText_->SetPos(0, 55);
	debugText_->Printf("isGrab:%d", IsGrab);
	debugText_->SetPos(0, 70);
	debugText_->Printf("angle:%f", worldTransform_.rotation_.z);
}

Vector3 PlayerHand::GetWorldPos()
{
	return worldTransform_.translation_;
}

void PlayerHand::OnCollision()
{
	IsGrab = true;
}

void PlayerHand::OnCollision2(Collider& collider)
{
}

void PlayerHand::ReachOut(const Vector3& pos, const float& angle)
{
	if (!IsUse)
	{
		worldTransform_.rotation_.z = angle;
		worldTransform_.UpdateMatrix();
		//終着点決める
		endPos.x = pos.x + cosf(worldTransform_.rotation_.z) * handLengthMax;
		endPos.y = pos.y + sinf(worldTransform_.rotation_.z) * handLengthMax;
		
		//移動用のベクトル
		velocity_.x = cosf(worldTransform_.rotation_.z) * handLengthMax;
		velocity_.y = sinf(worldTransform_.rotation_.z) * handLengthMax;

		//正規化
		velocity_.Normalized();
		
		IsUse = true;
		IsGo = true;

		ChangeState(new HandReachOut);
	}
}


void PlayerHand::ChangeState(HandState* state)
{
	delete this->state;
	this->state = state;
	state->SetHand(this);
}

//---------------------------------------------------------------
void HandState::SetHand(PlayerHand* playerHand)
{
	this->hand = playerHand;
}

//-----------------------------------
void HandNormal::Update()
{
	//何もしない
}

//------------------------------------
void HandReachOut::Update()
{
	//終着点に向かって動かす
	hand->SetWorldPos({ hand->GetWorldPos() + hand->velocity_ });

	//手が限界まで伸びたら
	Vector3 vec = hand->GetWorldPos() - hand->GetplayerPos();
	//戻す
	if (vec.GetLength() >= handLengthMax)
	{
		hand->SetIsGo(false);
		hand->SetIsBack(true);
	}
	//外部で当たり判定計算して、つかんだ判定になったら(そこでendposは設定されてる前提)
	if (hand->GetIsGrab())
	{
		hand->SetIsGo(false);
		hand->SetIsBack(false);
		hand->ChangeState(new HandGrab);
	}
	//戻しているときはplayerの方向に向かって
	else if (hand->GetIsBack())
	{
		//hand->SetEndPos(hand->GetplayerPos());
		//移動用のベクトル
		Vector3 vec = hand->GetplayerPos() - hand->GetWorldPos();
		hand->velocity_.x = vec.GetNormalized().x;
		hand->velocity_.y = vec.GetNormalized().y;

		//正規化
		hand->velocity_.Normalized();

		//手が戻ってきたら
		if (CollisionCircleCircle(hand->GetplayerPos(), hand->GetRadius(), hand->GetWorldPos(), hand->GetRadius()) && hand->GetIsBack())
		{
			hand->SetIsGo(false);
			hand->SetIsBack(false);
			hand->SetIsUse(false);
			hand->ChangeState(new HandNormal);
		}
	}
	
}

//------------------------------------
void HandGrab::Update()
{
	//手の位置に着いたら
	if (CollisionCircleCircle(hand->GetplayerPos(), hand->GetRadius(), hand->GetWorldPos(), hand->GetRadius()))
	{
 		hand->SetIsGrab(false);
		hand->SetIsUse(false);
		hand->ChangeState(new HandNormal);
	}
}


