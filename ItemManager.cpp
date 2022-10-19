#include "ItemManager.h"
#include <random>


void ItemManager::Initialize(Player* player, Model* model, uint32_t* textureHandle, HandStop* handStop, EffectManager* effectManager,
	GameSystem* gameSystem)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	this->player = player;
	this->effectManager = effectManager;
	this->gameSystem = gameSystem;
	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	this->handStop = handStop;
}

void ItemManager::ItemGenerate(const Vector3& pos)
{
	//敵を生成、初期化
	std::unique_ptr<Item> item = std::make_unique<Item>();
	item->Initialize(model_, textureHandle_, pos, handStop, effectManager);
	/*item->SetPlayer(player_);*/
	//敵を登録
	items.push_back(std::move(item));
}

void ItemManager::Update()
{
	if (!handStop->GetIsStop())
	{
		for (std::unique_ptr<Item>& item : items)
		{
			item->Update();
		}
	}
	//破壊されたら
	for (std::unique_ptr<Item>& item : items)
	{
		if (item.get()->GetIsDead())
		{
			//エフェクト
			effectManager->ParticleGenerate(item.get()->GetWorldPos(), { 890,140 });
			//ボーナスタイム追加
			gameSystem->SetBornusTime(gameSystem->GetBornusTime() + item.get()->GetBonusTime());
		}
	}
	//消す
	items.remove_if([](std::unique_ptr<Item>& item)
		{
			return (item->GetIsDead());
		}
	);
}

void ItemManager::Draw(const ViewProjection& view)
{
	for (std::unique_ptr<Item>& item : items)
	{
		item->Draw(view);
	}
}

