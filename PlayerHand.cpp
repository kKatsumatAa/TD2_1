#include "PlayerHand.h"

void PlayerHand::Initialize(Model* model, uint32_t* textureHandle, Wall* wall)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	this->wall = wall;
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	//�ŏ��͉������Ă��Ȃ�
	state = new HandNormal;
	state->SetHand(this);

	//radius_ = 2.0f;

	//�Փˑ���
	SetCollisionAttribute(kCollisionAttributeHand);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void PlayerHand::Update(const float& angle, const Vector3& playerPos)
{
	IsGrabOld = IsGrab;

	//�L�΂��Ă��Ȃ��Ƃ��ɉ�]�Aplayer�̈ʒu�ɒu��
	if (!IsUse) {
		worldTransform_.translation_ = playerPos;
		this->worldTransform_.rotation_.z = angle;
	}

	this->playerPos = playerPos;

	worldTransform_.UpdateMatrix();

	state->Update();
}

void PlayerHand::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_[1]);
}

Vector3 PlayerHand::GetWorldPos()
{
	return worldTransform_.translation_;
}

void PlayerHand::ResetFlag()
{
	IsUse = false;
	IsGrab = false;
	IsGrabOld = false;
	IsGo = false;
	IsBack = false;
}

void PlayerHand::OnCollision(Collider& collider)
{
	IsGrab = true;
}

void PlayerHand::OnCollision2(Collider& collider)
{
}

void PlayerHand::ReachOut(const Vector3& pos, const float& angle)
{
	if (1)
	{
		worldTransform_.rotation_.z = angle;
		worldTransform_.UpdateMatrix();
		//�I���_���߂�
		endPos.x = pos.x + cosf(worldTransform_.rotation_.z) * handLengthMax;
		endPos.y = pos.y + sinf(worldTransform_.rotation_.z) * handLengthMax;

		//�ړ��p�̃x�N�g��
		velocity_.x = cosf(worldTransform_.rotation_.z) * handLengthMax;
		velocity_.y = sinf(worldTransform_.rotation_.z) * handLengthMax;

		//���K��
		velocity_.Normalized();
		velocity_ *= handVelocityExtend;

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
	//�������Ȃ�
}

//------------------------------------
void HandReachOut::Update()
{
	//�O���œ����蔻��v�Z���āA���񂾔���ɂȂ�����(������endpos�͐ݒ肳��Ă�O��)
	if (hand->GetIsGrab())
	{
		hand->SetIsGo(false);
		hand->SetIsBack(false);
		hand->ChangeState(new HandGrab);
	}
	else
	{
		bool isWallGrab = false;
		//�I���_�Ɍ������ē�����
		hand->SetWorldPos(hand->GetWall()->isCollisionWall(hand->GetWorldPos(), hand->velocity_, &isWallGrab));
		if (isWallGrab == true) {
			hand->SetIsGrab(true);
		}

		//�肪���E�܂ŐL�т���
		Vector3 vec = hand->GetWorldPos() - hand->GetplayerPos();
		//�߂�
		if (vec.GetLength() >= handLengthMax)
		{
			hand->SetIsGo(false);
			hand->SetIsBack(true);
		}

		//�߂��Ă���Ƃ���player�̕����Ɍ�������
		if (hand->GetIsBack())
		{
			//hand->SetEndPos(hand->GetplayerPos());
			//�ړ��p�̃x�N�g��
			Vector3 vec = hand->GetplayerPos() - hand->GetWorldPos();
			hand->velocity_.x = vec.GetNormalized().x * handVelocityExtend;
			hand->velocity_.y = vec.GetNormalized().y * handVelocityExtend;

			//�肪�߂��Ă�����
			if (CollisionCircleCircle(hand->GetplayerPos(), hand->GetRadius(), hand->GetWorldPos(), hand->GetRadius()) && hand->GetIsBack())
			{
				hand->SetIsGo(false);
				hand->SetIsBack(false);
				hand->SetIsUse(false);
				hand->ChangeState(new HandNormal);
			}
		}
	}
}

//------------------------------------
void HandGrab::Update()
{
	//��̈ʒu�ɒ�������
	if (CollisionCircleCircle(hand->GetplayerPos(), hand->GetRadius(), hand->GetWorldPos(), hand->GetRadius()) || !hand->GetIsGrab())
	{
		hand->SetIsGrab(false);
		hand->SetIsUse(false);
		hand->ChangeState(new HandNormal);
	}
}


