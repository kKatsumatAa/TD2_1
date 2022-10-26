#include "Item.h"


void Item::Initialize(Model* model, uint32_t* textureHandle, const Vector3& pos, HandStop* handStop, EffectManager* effectManager,
	GameSystem* gameSystem)
{
	assert(model);

	model_ = Model::CreateFromOBJ("item", true);
	textureHandle_ = textureHandle;

	this->effectManager = effectManager;
	debugText_ = DebugText::GetInstance();

	this->gameSystem = gameSystem;

	isItem = true;

	this->handStop = handStop;

	bonusTime = 0;
	timer = 0;
	aliveNum = NULL;
	useHandCount = 0;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = { pi / 2,0,0 };
	worldTransform_.UpdateMatrix();

	//衝突属性
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributePlayer);

	bonusTime_ = new Number();
	bonusTime_->Initialize(textureHandle_[10]);
}

void Item::Update()
{
	timer++;

	if (timer % 180 == 0)
	{
		bonusTime++;
	}
}

void Item::Draw(const ViewProjection& view)
{
	model_->Draw(worldTransform_, view);

	debugText_->SetPos(10, 100);
	debugText_->Printf("bonusTime:%d", bonusTime);
}

void Item::DrawSprite() {
	bonusTime_->Draw(Convert2D(worldTransform_.translation_), { 0,0,0,255 }, bonusTime,8);
}

Vector3 Item::GetWorldPos()
{
	return worldTransform_.translation_;
}

void Item::OnCollision(Collider& collider)
{
	isDead = true;

	//重力の箱が当たった時のみボーナス
	if (collider.GetIsGravityObj())
	{
		//ボーナスタイム追加
		gameSystem->SetBornusTime(gameSystem->GetBornusTime() + GetBonusTime());
		gameSystem->SetTime((float)(gameSystem->GetTime() + GetBonusTime() * 60));
	}
}

void Item::OnCollision2(Collider& collider)
{
	//手の位置を敵の位置にする
	collider.SetWorldPos(worldTransform_.translation_);
	//
	if (collider.GetHandCount() == 0)
	{
		//掴まれている数をカウント
		SetHandCount(GetHandCount() + 1);
		collider.SetHandCount(1);
	}
}
