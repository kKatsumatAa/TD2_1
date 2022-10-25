#include "ItemManager.h"
#include <random>


void ItemManager::Initialize(Player* player, Model* model, uint32_t* textureHandle, HandStop* handStop, EffectManager* effectManager,
	GameSystem* gameSystem)
{
	assert(model);

	for (int i = 0; i < _countof(aliveItemNumber); i++)
	{
		aliveItemNumber[i] = NULL;
	}

	 groupCountI = 0;
	 items.clear();

	model_ = model;
	textureHandle_ = textureHandle;
	this->player = player;
	this->effectManager = effectManager;
	this->gameSystem = gameSystem;
	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	this->handStop = handStop;
}

void ItemManager::ItemGenerate(const Vector3& pos, int groupNum)
{
	//敵を生成、初期化
	std::unique_ptr<Item> item = std::make_unique<Item>();
	item->Initialize(model_, textureHandle_, pos, handStop, effectManager, gameSystem);
	item->SetAliveNum(groupNum);
	/*item->SetPlayer(player_);*/
	//敵を登録
	items.push_back(std::move(item));
}

void ItemManager::Update(Tutorial* tutorial)
{
	//csv用
	{
		int count[5] = { NULL };

		for (std::unique_ptr<Item>& item : items)
		{
			for (int i = 0; i < _countof(aliveItemNumber); i++)
			{
				if (item.get()->GetAliveNum() == aliveItemNumber[i] && aliveItemNumber[i] != 0)
				{
					if (count[i] == NULL)
					{
						count[i] = 0;
					}
					count[i]++;
				}
			}
		}

		//前に詰める
		for (int i = 0; i < _countof(aliveItemNumber) - 2; i++)
		{
			if (count[i] == 0)
			{
				if (aliveItemNumber[i + 1] != 0)
					aliveItemNumber[i] = aliveItemNumber[i + 1];
				else aliveItemNumber[i] = 0;

				if (aliveItemNumber[i + 2] == 0)
				{
					aliveItemNumber[i + 1] = 0;
				}
				if (count[i + 1] != 0)
					count[i] = count[i + 1];

				if (count[i + 2] == 0)
				{
					count[i + 1] = 0;
					break;
				}
			}
		}
	}

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

			if (tutorial != nullptr && tutorial->GetState() == ITEM)tutorial->AddStateNum();
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

void ItemManager::DrawSprite() {
	for (std::unique_ptr<Item>& item : items)
	{
		item->DrawSprite();
	}
}