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

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransformHand_.Initialize();
	worldTransformHand_.scale_ = { 0.2f,0.2f,0.2f };

	handR.Initialize(modelHand_, textureHandle, wall);
	handL.Initialize(modelHand_, textureHandle, wall);

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

	//使ってないときプレイヤーと一緒に移動
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

	//playerのusehandCountはスローモーション用（addHandCountがhandの二個同時掴み用）
	player->useHandCount = 0;

	if (player->input_->TriggerKey(DIK_SPACE))
	{
		//どっちも伸ばしていないときに使うのは絶対右手
		if (!player->GetHandR()->GetIsUse())
		{
			player->GetHandR()->ReachOut(player->GetWorldPos(), player->GetAngle() + pi / 2.0f);
			//使っている手の配列に登録
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

		//使っている手の更新処理
		player->GetUseHands()[0]->Update(player->GetAngle(), player->GetWorldPos());

		//小さい範囲こうげき生成
		if (player->GetUseHands()[0]->GetTriggerIsGrab())
		{
			player->GetSkillManager()->SkillGenerate(player->GetWorldPos(), 8.0f);
		}
		//二つ目を伸ばすとき、時間止める
		if (player->input_->TriggerKey(DIK_SPACE))
		{
			player->GetHandStop()->SetIsStop(true);
			player->useHandCount++;
		}
		else if (player->input_->ReleaseTriggerKey(DIK_SPACE) && player->useHandCount == 2)
		{
			//二つ目の手も使ったらstateを両手に変える
			if (!player->GetHandR()->GetIsUse())
			{
				//使ってる手を入れておく配列の２番目に入れる
				player->GetUseHands()[1] = player->GetHandR();
				player->GetHandR()->ReachOut(player->GetWorldPos(), player->GetAngle() + pi / 2.0f);
				player->ChangeState(new TwoHand);
			}
			else if (!player->GetHandL()->GetIsUse())
			{
				//使ってる手を入れておく配列の２番目に入れる
				player->GetUseHands()[1] = player->GetHandL();
				player->GetHandL()->ReachOut(player->GetWorldPos(), player->GetAngle() + pi / 2.0f);
				player->ChangeState(new TwoHand);
			}
		}
		//changeStateした後に参照するとエラー起きるので else if
		// //つかんだら
		else if (player->GetUseHands()[0]->GetIsGrab() &&
			(!CollisionCircleCircle(player->GetWorldPos(), player->GetRadius(),
				player->GetUseHands()[0]->GetWorldPos(), player->GetUseHands()[0]->GetRadius())))
		{
			Vector3 vec = player->GetUseHands()[0]->GetWorldPos() - player->GetWorldPos();
			vec.Normalized();

			player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * handVelocityExtend, &isWallHit));
		}
		// //突進し終わったら
		else if (!player->GetUseHands()[0]->GetIsUse()||isWallHit)
		{
			//手の掴んでいる数（両手づかみの判定用）
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

	//先に伸ばした手の更新処理
	player->GetUseHands()[0]->Update(player->GetAngle(), player->GetWorldPos());
	if (player->GetUseHands()[1] != nullptr && !player->GetUseHands()[1]->GetIsGrab() && player->GetUseHands()[1]->GetIsUse())
		player->GetUseHands()[1]->Update(player->GetAngle(), player->GetWorldPos());

	//外部で一体の敵を二つの手でつかんだ判定が出たら
	if (player->GetIsTwoHandOneGrab())
	{
		//両手掴みの貫通用のベクトル
 		Vector3 vec = player->GetUseHands()[0]->GetWorldPos() - player->GetWorldPos();
		vec.Normalized();
		player->SetVelocity(vec);

		player->ChangeState(new TwoHandOneGrab);
	}
	//つかんだら
	else if (player->GetUseHands()[0]->GetIsGrab())
	{
		Vector3 vec = player->GetUseHands()[0]->GetWorldPos() - player->GetWorldPos();
		vec.Normalized();

		player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * handVelocityExtend, &isWallHit));
	}
	//突進し終わったら
	else if (!player->GetUseHands()[0]->GetIsUse())
	{
		//２つ目の手がなければ
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
			//２つ目の手を１つ目に変更して、２つ目を無くす
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

	//同じ敵をつかんでいるので更新は片方のみ
	player->GetUseHands()[0]->Update(player->GetAngle(), player->GetWorldPos());
	player->GetUseHands()[1]->Update(player->GetAngle(), player->GetWorldPos());

	//つかんだら
	for (int i = 0; i < 2; i++)
	{
		if (player->GetUseHands()[i]->GetIsGrab())
		{
			vec = player->GetUseHands()[i]->GetWorldPos() - player->GetWorldPos();
			vec.Normalized();

			player->SetWorldPos(player->GetWall()->isCollisionWall(player->GetWorldPos(), vec * handVelocityExtend, &isWallHit));
		}
	}
	//突進し終わったら(or壁に当たったら)
	if ((!player->GetUseHands()[0]->GetIsUse() && !player->GetUseHands()[1]->GetIsUse()) || isWallHit)
	{
		//貫通突進
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

	//三回小さい範囲こうげき
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
	//追加するかも
}


