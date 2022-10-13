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
	this->handStop = handStop;

	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransformHand_.Initialize();
	worldTransformHand_.scale_ = { 0.2f,0.2f,0.2f };

	handR.Initialize(modelHand_, textureHandle, wall);
	handL.Initialize(modelHand_, textureHandle, wall);

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

	if (input_->ReleaseTriggerKey(DIK_SPACE))
	{
		GetHandStop()->SetIsStop(false);
	}

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

	//player��usehandCount�̓X���[���[�V�����p�iaddHandCount��hand�̓�����͂ݗp�j
	player->useHandCount = 0;

	if (player->input_->TriggerKey(DIK_SPACE))
	{
		//�ǂ������L�΂��Ă��Ȃ��Ƃ��Ɏg���̂͐�ΉE��
		if (!player->GetHandR()->GetIsUse())
		{
			player->GetHandR()->ReachOut(player->GetWorldPos(), player->GetAngle() + pi / 2.0f);
			//�g���Ă����̔z��ɓo�^
			player->GetUseHands()[0] = (player->GetHandR());
			player->useHandCount++;
			player->ChangeState(new OneHandOneGrab);
		}
	}
}

//---------------------------------
void OneHandOneGrab::Update()
{
	if (!player->GetHandStop()->GetIsStop())
	{
		bool isWallHit = false;

		//�g���Ă����̍X�V����
		player->GetUseHands()[0]->Update(player->GetAngle(), player->GetWorldPos());

		//�������͈͂�����������
		if (player->GetUseHands()[0]->GetTriggerIsGrab())
		{
			player->GetSkillManager()->SkillGenerate(player->GetWorldPos(), 8.0f);
		}
		//��ڂ�L�΂��Ƃ��A���Ԏ~�߂�
		if (player->input_->TriggerKey(DIK_SPACE))
		{
			player->GetHandStop()->SetIsStop(true);
			player->useHandCount++;
		}
		else if (player->input_->ReleaseTriggerKey(DIK_SPACE) && player->useHandCount == 2)
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
		// //���񂾂�
		else if (player->GetUseHands()[0]->GetIsGrab() &&
			(!CollisionCircleCircle(player->GetWorldPos(), player->GetRadius(),
				player->GetUseHands()[0]->GetWorldPos(), player->GetUseHands()[0]->GetRadius())))
		{
			Vector3 vec = player->GetUseHands()[0]->GetWorldPos() - player->GetWorldPos();
			vec.Normalized();

			player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * handVelocityExtend, &isWallHit));
		}
		// //�ːi���I�������
		else if (!player->GetUseHands()[0]->GetIsUse()||isWallHit)
		{
			//��̒͂�ł��鐔�i����Â��݂̔���p�j
			player->GetUseHands()[0]->SetHandCount(0);
			player->GetUseHands()[0]->ResetFlag();
			player->GetUseHands()[0] = nullptr;
			player->ChangeState(new NoGrab);
		}
	}
}

//---------------------------------
void TwoHand::Update()
{
	bool isWallHit = false;

	//��ɐL�΂�����̍X�V����
	player->GetUseHands()[0]->Update(player->GetAngle(), player->GetWorldPos());
	if (player->GetUseHands()[1] != nullptr && !player->GetUseHands()[1]->GetIsGrab() && player->GetUseHands()[1]->GetIsUse())
		player->GetUseHands()[1]->Update(player->GetAngle(), player->GetWorldPos());

	//�O���ň�̂̓G���̎�ł��񂾔��肪�o����
	if (player->GetIsTwoHandOneGrab())
	{
		//����݂͂̊ђʗp�̃x�N�g��
 		Vector3 vec = player->GetUseHands()[0]->GetWorldPos() - player->GetWorldPos();
		vec.Normalized();
		player->SetVelocity(vec);

		player->ChangeState(new TwoHandOneGrab);
	}
	//���񂾂�
	else if (player->GetUseHands()[0]->GetIsGrab())
	{
		Vector3 vec = player->GetUseHands()[0]->GetWorldPos() - player->GetWorldPos();
		vec.Normalized();

		player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * handVelocityExtend, &isWallHit));
	}
	//�ːi���I�������
	else if (!player->GetUseHands()[0]->GetIsUse())
	{
		//�Q�ڂ̎肪�Ȃ����
		if (player->GetUseHands()[1] == nullptr || isWallHit)
		{
			player->GetUseHands()[0]->SetHandCount(0);
			player->GetUseHands()[0]->ResetFlag();
			player->GetUseHands()[0] = nullptr;

			player->useHandCount = 0;
			player->ChangeState(new NoGrab);
		}
		else
		{
			//�Q�ڂ̎���P�ڂɕύX���āA�Q�ڂ𖳂���
			player->GetUseHands()[0]->SetHandCount(0);
			player->GetUseHands()[0]->ResetFlag();
			player->GetUseHands()[0] = player->GetUseHands()[1];
			player->GetUseHands()[1] = nullptr;
			player->useHandCount--;
			player->ChangeState(new OneHandOneGrab);
		}
	}
}

//---------------------------------
void TwoHandOneGrab::Update()
{
	bool isWallHit = false;

	Vector3 vec;

	//�����G������ł���̂ōX�V�͕Е��̂�
	player->GetUseHands()[0]->Update(player->GetAngle(), player->GetWorldPos());
	player->GetUseHands()[1]->Update(player->GetAngle(), player->GetWorldPos());

	//���񂾂�
	for (int i = 0; i < 2; i++)
	{
		if (player->GetUseHands()[i]->GetIsGrab())
		{
			vec = player->GetUseHands()[i]->GetWorldPos() - player->GetWorldPos();
			vec.Normalized();

			player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * handVelocityExtend, &isWallHit));
		}
	}
	//�ːi���I�������(or�ǂɓ���������)
	if ((!player->GetUseHands()[0]->GetIsUse() && !player->GetUseHands()[1]->GetIsUse()) || isWallHit)
	{
		//�ђʓːi
		player->GetUseHands()[0]->SetHandCount(0);
		player->GetUseHands()[1]->SetHandCount(0);

		player->GetUseHands()[0]->ResetFlag();
		player->GetUseHands()[1]->ResetFlag();

		player->GetUseHands()[0]->ChangeState(new HandNormal);
		player->GetUseHands()[1]->ChangeState(new HandNormal);

		player->GetUseHands()[0] = nullptr;
		player->GetUseHands()[1] = nullptr;

		player->useHandCount = 0;
		player->SetIsTwoHandOneGrab(false);

		player->ChangeState(new TwoHandOneGrab2);
	}
}

void TwoHandOneGrab2::Update()
{
	timer++;

	player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), player->GetVelocity()));

	//�O�񏬂����͈͂�������
	if (timer % (maxTimer / 3) == 0)
	{
		player->GetSkillManager()->SkillGenerate(player->GetWorldPos(), 1.0f);
	}

	if (timer >= maxTimer)
	{
		player->ChangeState(new NoGrab);
	}
}

//---------------------------------
void TwoHandTwoGrab::Update()
{
	//�ǉ����邩��
}


