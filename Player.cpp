#include "Player.h"

void Player::ChangeState(PlayerHandState* state)
{
	delete this->state;
	this->state = state;
	state->SetPlayer(this);
}

//----------------------------------------------------------------------
void Player::Initialize(Model* model, uint32_t* textureHandle, HandSkillManager* skillManager)
{
	assert(model);

	model_ = model;
	modelHand_ = model;
	textureHandle_ = textureHandle;

	this->skillManager = skillManager;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransformHand_.Initialize();
	worldTransformHand_.scale_ = { 0.2f,0.2f,0.2f };

	handR.Initialize(modelHand_, textureHandle);
	handL.Initialize(modelHand_, textureHandle);

	state = new NoGrab;
	state->SetPlayer(this);

	//�Փˑ���
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void Player::Update()
{
	//player�̉�]
	worldTransform_.rotation_.z += ((int)input_->PushKey(DIK_LEFTARROW) - (int)input_->PushKey(DIK_RIGHTARROW)) * 0.05f;
	worldTransform_.UpdateMatrix();

	//��̓͂��͈͗p
	worldTransformHand_.translation_.x = worldTransform_.translation_.x + cosf(worldTransform_.rotation_.z + pi / 2.0f) * handLengthMax;
	worldTransformHand_.translation_.y = worldTransform_.translation_.y + sinf(worldTransform_.rotation_.z + pi / 2.0f) * handLengthMax;
	worldTransformHand_.UpdateMatrix();

	//�g���ĂȂ��Ƃ��v���C���[�ƈꏏ�Ɉړ�
	if (!handR.GetIsUse()) handR.Update(worldTransform_.rotation_.z, worldTransform_.translation_);
	if (!handL.GetIsUse()) handL.Update(worldTransform_.rotation_.z, worldTransform_.translation_);

	state->Update();
}

void Player::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_[0]);
	modelHand_->Draw(worldTransformHand_, view, textureHandle_[0]);

	handR.Draw(view);
	handL.Draw(view);
}

void Player::ReachOut()
{
}

Vector3 Player::GetWorldPos()
{
	return worldTransform_.translation_;
}

void Player::OnCollision()
{
}

void Player::OnCollision2(Collider& collider)
{
}


//--------------------------------------------------------------------------
void PlayerHandState::SetPlayer(Player* player)
{
	this->player = player;
}

//---------------------------------
void NoGrab::Update()
{
	player->GetHandR()->Update(player->GetAngle() + pi / 2.0f, player->GetWorldPos());
	player->GetHandL()->Update(player->GetAngle() + pi / 2.0f, player->GetWorldPos());

	if (player->input_->TriggerKey(DIK_SPACE))
	{
		//�ǂ������L�΂��Ă��Ȃ��Ƃ��Ɏg���̂͐�ΉE��
		if (!player->GetHandR()->GetIsUse())
		{
			player->GetHandR()->ReachOut(player->GetWorldPos(), player->GetAngle() + pi / 2.0f);
			//�g���Ă����̔z��ɓo�^
			player->GetUseHands()[0] = (player->GetHandR());
			player->ChangeState(new OneHandOneGrab);
		}
	}
}

//---------------------------------
void OneHandOneGrab::Update()
{
	//�g���Ă����̍X�V����

	player->GetUseHands()[0]->Update(player->GetAngle(), player->GetWorldPos());

	//�������͈͂�����������
	if (player->GetUseHands()[0]->GetTriggerIsGrab())
	{	
		player->GetSkillManager()->SkillGenerate(player->GetWorldPos());
	}

	if (player->input_->TriggerKey(DIK_SPACE))
	{
		//��ڂ̎���g������state�𗼎�ɕς���
		if (!player->GetHandR()->GetIsUse())
		{
			//�g���Ă������Ă����z��̂Q�Ԗڂɓ����
			player->GetUseHands()[1] = player->GetHandR();
			player->GetHandR()->ReachOut(player->GetWorldPos(), player->GetAngle() + pi / 2.0f);
			player->ChangeState(new TwoHand);
		}
		else if (!player->GetHandL()->GetIsUse())
		{
			//�g���Ă������Ă����z��̂Q�Ԗڂɓ����
			player->GetUseHands()[1] = player->GetHandL();
			player->GetHandL()->ReachOut(player->GetWorldPos(), player->GetAngle() + pi / 2.0f);
			player->ChangeState(new TwoHand);
		}
	}
	//changeState������ɎQ�Ƃ���ƃG���[�N����̂� else if
	//���񂾂�
	else if (player->GetUseHands()[0]->GetIsGrab())
	{
		Vector3 vec = player->GetUseHands()[0]->GetWorldPos() - player->GetWorldPos();
		vec.Normalized();

		player->SetWorldPos(player->GetWorldPos() + vec);
	}
	//�ːi���I�������
	else if (!player->GetUseHands()[0]->GetIsUse())
	{
		player->ChangeState(new NoGrab);
	}
}

//---------------------------------
void TwoHand::Update()
{
	//��ɐL�΂�����̍X�V����
	player->GetUseHands()[0]->Update(player->GetAngle(), player->GetWorldPos());
	if (player->GetUseHands()[1] != nullptr && !player->GetUseHands()[1]->GetIsGrab() && player->GetUseHands()[1]->GetIsUse())
		player->GetUseHands()[1]->Update(player->GetAngle(), player->GetWorldPos());

	//���񂾂�
	if (player->GetUseHands()[0]->GetIsGrab())
	{
		Vector3 vec = player->GetUseHands()[0]->GetWorldPos() - player->GetWorldPos();
		vec.Normalized();

		player->SetWorldPos(player->GetWorldPos() + vec);
	}
	//�ːi���I�������
	else if (!player->GetUseHands()[0]->GetIsUse())
	{
		//�Q�ڂ̎肪�Ȃ����
		if (player->GetUseHands()[1] == nullptr)
		{
			player->ChangeState(new NoGrab);
		}
		else
		{
			//�Q�ڂ̎���P�ڂɕύX���āA�Q�ڂ𖳂���
			player->GetUseHands()[0] = player->GetUseHands()[1];
			player->GetUseHands()[1] = nullptr;
			player->ChangeState(new OneHandOneGrab);
		}
	}
}

//---------------------------------
void TwoHandOneGrab::Update()
{
}

//---------------------------------
void TwoHandTwoGrab::Update()
{
}
