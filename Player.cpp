#include "Player.h"

void Player::ChangeState(PlayerHandState* state)
{
	delete this->state;
	this->state = state;
	state->SetPlayer(this);
}

//----------------------------------------------------------------------
void Player::Initialize(Model* model, Model* guideM, uint32_t* textureHandle, HandSkillManager* skillManager, HandStop* handStop,
	Wall* wall, Gravity* gravity, Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle)
{
	assert(model);

	velocity = { 0,0,0 };

	//角度
	angle = 0.0f;

	//貫通攻撃かどうか
	isRush = false;
	isRush2 = false;

	model_ = model;
	modelGuide_ = guideM;
	textureHandle_ = textureHandle;

	isPlayer = true;

	this->skillManager = skillManager;
	this->handStop = handStop;

	this->wall = wall;
	this->gravity = gravity;
	guide = new Guide;

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;

	guide->Initialize(modelGuide_, textureHandle_);

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,-15,0 };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0,0,pi / 2 };
	worldTransform_.UpdateMatrix();
	worldTransformHand_.Initialize();
	//worldTransformHand_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransformHand2_.Initialize();
	//worldTransformHand2_.scale_ = { 1.0f,1.0f,1.0f };

	handR.Initialize(model_, textureHandle, wall);

	state = new NoGrab;
	state->SetPlayer(this);

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void Player::Update(GameSystem* gameSystem, bool isEffect, Tutorial* tutorial)
{
	this->isEffect = isEffect;
	if (gameSystem->GetIsStageChange())
	{
		worldTransform_.translation_ = { stageLeftTop.x + 18, stageLeftTop.y + radius_,0 };

		switch (gameSystem->GetStage())
		{
		case 1:
			worldTransform_.translation_ = { stageLeftTop.x + 18, stageLeftTop.y + radius_,0 };
			break;
		case 2:
			worldTransform_.translation_ = { stageLeftTop.x + 18, stageLeftTop.y + radius_,0 };
			break;
		case 3:
			worldTransform_.translation_ = { stageLeftTop.x + 18, stageLeftTop.y + radius_,0 };
			break;
		case 4:
			worldTransform_.translation_ = { stageLeftTop.x + 18, stageLeftTop.y + radius_,0 };
			break;
		case 5:
			worldTransform_.translation_ = { stageLeftTop.x + 18, stageLeftTop.y + radius_,0 };
			break;
		}
	}

	if (isRush || isRush2) guide->SetIsLongPush(true);
	else                   guide->SetIsLongPush(false);

	//playerの回転
	worldTransform_.rotation_.z += ((int)input_->PushKey(DIK_LEFTARROW) - (int)input_->PushKey(DIK_RIGHTARROW)) * 0.05f;
	worldTransform_.UpdateMatrix();

	//手の届く範囲用
	worldTransformHand_.translation_.x = worldTransform_.translation_.x + cosf(worldTransform_.rotation_.z + pi / 2.0f) * handLengthMax;
	worldTransformHand_.translation_.y = worldTransform_.translation_.y + sinf(worldTransform_.rotation_.z + pi / 2.0f) * handLengthMax;
	worldTransformHand_.UpdateMatrix();
	worldTransformHand2_.translation_.x = worldTransform_.translation_.x + cosf(worldTransform_.rotation_.z + pi / 2.0f);
	worldTransformHand2_.translation_.y = worldTransform_.translation_.y + sinf(worldTransform_.rotation_.z + pi / 2.0f);
	worldTransformHand2_.translation_.Normalized();
	//worldTransformHand2_.UpdateMatrix();

	guide->Update(worldTransform_.translation_, Vector3(cosf(worldTransform_.rotation_.z + pi / 2.0f), sinf(worldTransform_.rotation_.z + pi / 2.0f), 0));

	//使ってないときプレイヤーと一緒に移動
	if (!handR.GetIsUse()) handR.Update(worldTransform_.rotation_.z, worldTransform_.translation_);

	state->Update(tutorial);
}

void Player::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view);
	//modelHand_->Draw(worldTransformHand_, view, textureHandle_[0]);
	//modelHand_->Draw(worldTransformHand2_, view, textureHandle_[0]);


	//handR.Draw(view);
}

void Player::ReachOut()
{
}

Vector3 Player::GetWorldPos()
{
	return worldTransform_.translation_;
}

void Player::OnCollision(Collider& collider)
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
void NoGrab::Update(Tutorial* tutorial)
{
	player->SetIsRush(false);
	player->SetIsRush2(false);

	//重力を適応
	player->SetWorldPos(player->gravity->Move(player->GetWorldPos(), 0.2f, 0.23f));

	if (player->input_->TriggerKey(DIK_SPACE) && player->isEffect == false)
	{
		player->ChangeState(new OneHandRushGrab);
	}
}


//---------------------------------
void OneHandRushGrab::Update(Tutorial* tutorial)
{
	player->SetIsRush(true);

	bool sugitaIsGomi = false;

	if (player->input_->PushKey(DIK_SPACE))
	{
		player->gravity->SetSugitaIsGomi(true);
		player->handStop->SetIsStop(true);
	}

	//重力を適応
	player->SetWorldPos(player->gravity->Move(player->GetWorldPos(), 0.2f, 0.23f));

	//掴んでいる状態でspace離したら
	if (!player->input_->PushKey(DIK_SPACE))
	{
		//player->gravity->SetSugitaIsGomi(false);
		player->handStop->SetIsStop(false);

		//if (tutorial != nullptr && tutorial->GetState() == LONG_PUSH) tutorial->AddStateNum();

		player->voiceHandle[0] = player->audio->PlayWave(player->soundDataHandle[0]);

		player->ChangeState(new OneHandRushAttack);
	}
}

void OneHandRushAttack::Update(Tutorial* tutorial)
{
	player->SetIsRush2(true);
	player->SetIsRush(false);
	bool isWallHit = false;

	Vector3 vec = { cosf(player->GetAngle() + pi / 2.0f),sinf(player->GetAngle() + pi / 2.0f),0 };
	vec.Normalized();

	player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec, &isWallHit));

	player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * 0.25, &isWallHit));

	//突進し終わったら
	if (isWallHit)
	{
		//両手掴みの貫通用のベクトル
		Vector3 vec = { cosf(player->GetAngle() + pi / 2.0f),sinf(player->GetAngle() + pi / 2.0f),0 };
		vec.Normalized();
		player->SetVelocity(vec);
		player->ChangeState(new OneHandRushAttack2);
	}
}

void OneHandRushAttack2::Update(Tutorial* tutorial)
{
	bool isWallHit = false;

	//移動処理
	player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), player->GetVelocity() * 1.5f, &isWallHit));


	////三回小さい範囲こうげき
	//if (timer % (maxTimer / 5) == 0)
	//{
	//	player->GetSkillManager()->SkillGenerate(player->GetWorldPos(), 1.0f);
	//}

	timer++;

	if (isWallHit)
	{
		if (tutorial != nullptr &&
			((tutorial->GetState() == LONG_PUSH && tutorial->GetStateNum() == 0)
				|| tutorial->GetState() == RUSH)) tutorial->AddStateNum();

		player->GetHandR()->ResetFlag();
		player->ChangeState(new NoGrab);
	}
}
