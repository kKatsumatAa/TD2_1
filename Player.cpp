#include "Player.h"

void Player::ChangeState(PlayerHandState* state)
{
	delete this->state;
	this->state = state;
	state->SetPlayer(this);
}

//----------------------------------------------------------------------
void Player::Initialize(Model* model, uint32_t* textureHandle, HandSkillManager* skillManager, HandStop* handStop,
	Wall* wall)
{
	assert(model);

	model_ = model;
	modelHand_ = model;
	textureHandle_ = textureHandle;
	this->wall = wall;

	this->skillManager = skillManager;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransformHand_.Initialize();
	worldTransformHand_.scale_ = { 0.2f,0.2f,0.2f };

	handR.Initialize(modelHand_, textureHandle, wall);

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

	state->Update();
}

void Player::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_[0]);
	modelHand_->Draw(worldTransformHand_, view, textureHandle_[0]);

	handR.Draw(view);
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

	if (player->input_->TriggerKey(DIK_SPACE))
	{
		//�ǂ������L�΂��Ă��Ȃ��Ƃ��Ɏg���̂͐�ΉE��
		if (!player->GetHandR()->GetIsUse())
		{
			player->GetHandR()->ReachOut(player->GetWorldPos(), player->GetAngle() + pi / 2.0f);

			player->ChangeState(new OneHandOneGrab);
		}
	}
}

//---------------------------------
void OneHandOneGrab::Update()
{
	//�g���Ă����̍X�V����
	player->GetHandR()->Update(player->GetAngle(), player->GetWorldPos());

	//�͂�ł����Ԃ�space�����Ă�����
	if (player->GetHandR()->GetIsGrab() && player->input_->PushKey(DIK_SPACE))
	{
		player->ChangeState(new OneHandRushGrab);
	}
	//��������Ȃ������畁�ʂ̓ːi
	else if (player->GetHandR()->GetIsGrab())
	{
		player->ChangeState(new OneHandAttack);
	}
}

//---------------------------------
void OneHandAttack::Update()
{
	bool isWallHit = false;

	//�g���Ă����̍X�V����
	player->GetHandR()->Update(player->GetAngle(), player->GetWorldPos());

	//�ړ�����
	if (player->GetHandR()->GetIsGrab() &&
		(!CollisionCircleCircle(player->GetWorldPos(), player->GetRadius(),
			player->GetHandR()->GetWorldPos(), player->GetHandR()->GetRadius()))) {
		Vector3 vec = player->GetHandR()->GetWorldPos() - player->GetWorldPos();
		vec.Normalized();

		player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * handVelocityExtend, &isWallHit));
	}

	//�ːi���I�������
	if (!player->GetHandR()->GetIsUse() || isWallHit)
	{
		player->GetHandR()->ResetFlag();
		player->ChangeState(new NoGrab);
	}
}

//---------------------------------
void OneHandRushGrab::Update()
{
	//�g���Ă����̍X�V����
	player->GetHandR()->Update(player->GetAngle(), player->GetWorldPos());

	//�͂�ł����Ԃ�space��������
	if (player->input_->ReleaseTriggerKey(DIK_SPACE))
	{
		player->ChangeState(new OneHandRushAttack);
	}
}

void OneHandRushAttack::Update()
{
	bool isWallHit = false;

	//�g���Ă����̍X�V����
	player->GetHandR()->Update(player->GetAngle(), player->GetWorldPos());

	//�ړ�����
	if (player->GetHandR()->GetIsGrab() &&
		(!CollisionCircleCircle(player->GetWorldPos(), player->GetRadius(),
			player->GetHandR()->GetWorldPos(), player->GetHandR()->GetRadius()))) {
		Vector3 vec = player->GetHandR()->GetWorldPos() - player->GetWorldPos();
		vec.Normalized();

		player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * handVelocityExtend, &isWallHit));
	}

	//�ːi���I�������
	if (!player->GetHandR()->GetIsUse() || isWallHit)
	{
		//����݂͂̊ђʗp�̃x�N�g��
		Vector3 vec = player->GetHandR()->GetWorldPos() - player->GetWorldPos();
		vec.Normalized();
		player->SetVelocity(vec);
		player->ChangeState(new OneHandRushAttack2);
	}
}

void OneHandRushAttack2::Update()
{
	bool isWallHit = false;

	timer++;

	//�g���Ă����̍X�V����
	player->GetHandR()->Update(player->GetAngle(), player->GetWorldPos());

	//�ړ�����
	player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), player->GetVelocity(), &isWallHit));


	//�O�񏬂����͈͂�������
	if (timer % (maxTimer / 3) == 0)
	{
		player->GetSkillManager()->SkillGenerate(player->GetWorldPos(), 1.0f);
	}

	//�ːi���I�������
	if (timer >= maxTimer)
	{
		player->GetHandR()->ResetFlag();
		player->ChangeState(new NoGrab);
	}
	//���ꂩ�ǂɓ���������
	else if (isWallHit)
	{
		player->GetHandR()->ResetFlag();
		player->ChangeState(new NoGrab);
	}
}
