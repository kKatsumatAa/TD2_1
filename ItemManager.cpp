#include "ItemManager.h"
#include <random>


void ItemManager::Initialize(Player* player, Model* model, uint32_t* textureHandle, HandStop* handStop, EffectManager* effectManager)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	this->player = player;
	this->effectManager = effectManager;
	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	this->handStop = handStop;

	//仮
	for (int i = 0; i < 1; i++)
	{
		ItemGenerate({ posDist(engine),posDist(engine),0 });
	}
}

void ItemManager::ItemGenerate(const Vector3& pos)
{
	//敵を生成、初期化
	std::unique_ptr<Item> item = std::make_unique<Item>();
	item->Initialize(model_, textureHandle_, pos, handStop,effectManager);
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

	//敵一体に二つの手がついていたら
	for (std::unique_ptr<Item>& item : items)
	{
		if (item.get()->GetIsDead())effectManager->ParticleGenerate(item.get()->GetWorldPos(), { 1000,10 });

		//stateを変える
		if (item.get()->GetHandCount() >= 2 && !player->GetIsTwoHandOneGrab())
		{
			player->SetIsTwoHandOneGrab(true);
		}
	}

	//敵消す
	items.remove_if([](std::unique_ptr<Item>& item)
		{
			return (item->GetIsDead());
		}
	);

	//仮
	if (input_->TriggerKey(DIK_Z) || items.size() <= 0)
	{
		items.clear();
		for (int i = 0; i < 1; i++)
		{
			ItemGenerate({ posDist(engine),posDist(engine),0 });
		}
	}
}

void ItemManager::Draw(const ViewProjection& view)
{
	for (std::unique_ptr<Item>& item : items)
	{
		item->Draw(view);
	}
}

