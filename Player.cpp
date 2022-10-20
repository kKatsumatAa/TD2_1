#include "Player.h"

void Player::ChangeState(PlayerHandState* state)
{
	delete this->state;
	this->state = state;
	state->SetPlayer(this);
}

//----------------------------------------------------------------------
void Player::Initialize(Model* model, uint32_t* textureHandle, HandSkillManager* skillManager, HandStop* handStop,
	Wall* wall, Gravity* gravity)
{
	assert(model);

	model_ = model;
	modelHand_ = model;
	textureHandle_ = textureHandle;

	this->skillManager = skillManager;

	this->wall = wall;
	this->gravity = gravity;


	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 3.0f,3.0f,3.0f };
	worldTransformHand_.Initialize();
	//worldTransformHand_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransformHand2_.Initialize();
	//worldTransformHand2_.scale_ = { 1.0f,1.0f,1.0f };

	handR.Initialize(modelHand_, textureHandle, wall);

	state = new NoGrab;
	state->SetPlayer(this);

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);
}

void Player::Update()
{
	//playerの回転
	worldTransform_.rotation_.z += ((int)input_->PushKey(DIK_LEFTARROW) - (int)input_->PushKey(DIK_RIGHTARROW)) * 0.05f;
	worldTransform_.UpdateMatrix();

	//手の届く範囲用
	worldTransformHand_.translation_.x = worldTransform_.translation_.x + cosf(worldTransform_.rotation_.z + pi / 2.0f) * handLengthMax;
	worldTransformHand_.translation_.y = worldTransform_.translation_.y + sinf(worldTransform_.rotation_.z + pi / 2.0f) * handLengthMax;
	worldTransformHand_.UpdateMatrix();
	worldTransformHand2_.translation_.x = worldTransform_.translation_.x + cosf(worldTransform_.rotation_.z + pi / 2.0f);
	worldTransformHand2_.translation_.y = worldTransform_.translation_.y + sinf(worldTransform_.rotation_.z + pi / 2.0f);
	worldTransformHand2_.UpdateMatrix();

	//使ってないときプレイヤーと一緒に移動
	if (!handR.GetIsUse()) handR.Update(worldTransform_.rotation_.z, worldTransform_.translation_);

	state->Update();
}

void Player::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view, textureHandle_[0]);
	modelHand_->Draw(worldTransformHand_, view, textureHandle_[0]);
	modelHand_->Draw(worldTransformHand2_, view, textureHandle_[0]);

	debugText_->SetPos(10, 400);
	debugText_->Printf("isRush:%d", isRush);

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
	player->SetIsRush(false);
	player->SetIsRush2(false);

	//重力を適応
	player->SetWorldPos(player->gravity->Move(player->GetWorldPos(), 0.2f, 0.25f));

	if (player->input_->TriggerKey(DIK_SPACE))
	{
		player->ChangeState(new OneHandRushGrab);
	}
}

//---------------------------------
//void OneHandOneGrab::Update()
//{
//	//重力を適応
//	player->SetWorldPos(player->gravity->Move(player->GetWorldPos(), 0.2f, 0.1f));
//
//	//使っている手の更新処理
//	player->GetHandR()->Update(player->GetAngle(), player->GetWorldPos());
//
//	//スペース離したら貫通しなくなる
//	if (!player->input_->PushKey(DIK_SPACE))
//	{
//		isNotHandRushAttack = true;
//	}
//
//	//掴んでいる状態で手に当たったら
//	if (!player->GetHandR()->GetIsUse())
//	{
//		player->GetHandR()->ResetFlag();
//		player->ChangeState(new NoGrab);
//	}
//	//長押ししていたら
//	else if (player->GetHandR()->GetIsGrab() && isNotHandRushAttack == false)
//	{
//		player->ChangeState(new OneHandRushGrab);
//	}
//	//そうじゃなかったら普通の突進
//	else if (player->GetHandR()->GetIsGrab())
//	{
//		player->ChangeState(new OneHandAttack);
//	}
//}

//---------------------------------
//void OneHandAttack::Update()
//{
//	bool isWallHit = false;
//
//	//使っている手の更新処理
//	player->GetHandR()->Update(player->GetAngle(), player->GetWorldPos());
//
//	//移動処理
//	if (player->GetHandR()->GetIsGrab() &&
//		(!CollisionCircleCircle(player->GetWorldPos(), player->GetRadius(),
//			player->GetHandR()->GetWorldPos(), player->GetHandR()->GetRadius()))) {
//		Vector3 vec = player->GetHandR()->GetWorldPos() - player->GetWorldPos();
//		vec.Normalized();
//
//		player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * handVelocityExtend, &isWallHit));
//	}
//
//	//突進し終わったら
//	if (!player->GetHandR()->GetIsUse() || isWallHit)
//	{
//		player->GetHandR()->ResetFlag();
//		player->ChangeState(new NoGrab);
//	}
//}

//---------------------------------
void OneHandRushGrab::Update()
{
	player->SetIsRush(true);

	bool sugitaIsGomi = false;

	if (player->input_->PushKey(DIK_SPACE))
	{
		player->gravity->SetSugitaIsGomi(true);
	}

	//重力を適応
	player->SetWorldPos(player->gravity->Move(player->GetWorldPos(), 0.2f, 0.25f));

	//掴んでいる状態でspace離したら
	if (!player->input_->PushKey(DIK_SPACE))
	{
		//player->gravity->SetSugitaIsGomi(false);
		player->ChangeState(new OneHandRushAttack);
	}
}

void OneHandRushAttack::Update()
{
	player->SetIsRush2(true);
	player->SetIsRush(false);
	bool isWallHit = false;

	Vector3 vec = { cosf(player->GetAngle() + pi / 2.0f),sinf(player->GetAngle() + pi / 2.0f),0 };
	vec.Normalized();

	player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec, &isWallHit));

		player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * 1.25, &isWallHit));
	}

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

void OneHandRushAttack2::Update()
{
	bool isWallHit = false;

	//移動処理
	player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), player->GetVelocity() * 1.5f, &isWallHit));


	//三回小さい範囲こうげき
	if (timer % (maxTimer / 5) == 0)
	{
		player->GetSkillManager()->SkillGenerate(player->GetWorldPos(), 1.0f);
	}

	timer++;

	if (isWallHit)
	{
		player->GetHandR()->ResetFlag();
		player->ChangeState(new NoGrab);
	}
}
