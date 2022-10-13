#include "ItemManager.h"
#include <random>


void ItemManager::Initialize(Player* player, Model* model, uint32_t* textureHandle, HandStop* handStop, EffectManager* effectManager)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	this->player = player;
	this->effectManager = effectManager;
	//ƒVƒ“ƒOƒ‹ƒgƒ“ƒCƒ“ƒXƒ^ƒ“ƒX‚ğæ“¾
	input_ = Input::GetInstance();
	this->handStop = handStop;

	//‰¼
	for (int i = 0; i < 1; i++)
	{
		ItemGenerate({ posDist(engine),posDist(engine),0 });
	}
}

void ItemManager::ItemGenerate(const Vector3& pos)
{
	//“G‚ğ¶¬A‰Šú‰»
	std::unique_ptr<Item> item = std::make_unique<Item>();
	item->Initialize(model_, textureHandle_, pos, handStop,effectManager);
	/*item->SetPlayer(player_);*/
	//“G‚ğ“o˜^
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
	//“Gˆê‘Ì‚É“ñ‚Â‚Ìè‚ª‚Â‚¢‚Ä‚¢‚½‚ç
	for (std::unique_ptr<Item>& item : items)
	{
		if (item.get()->GetIsDead())effectManager->ParticleGenerate(item.get()->GetWorldPos(), { 1000,10 });
	}
	//“GÁ‚·
	items.remove_if([](std::unique_ptr<Item>& item)
		{
			return (item->GetIsDead());
		}
	);
	//“Gˆê‘Ì‚É“ñ‚Â‚Ìè‚ª‚Â‚¢‚Ä‚¢‚½‚ç
	for (std::unique_ptr<Item>& item : items)
	{
		//state‚ğ•Ï‚¦‚é
		if (item.get()->GetHandCount() == 2 && !player->GetIsTwoHandOneGrab())
		{
			player->SetIsTwoHandOneGrab(true);
		}
	}

	

	//‰¼
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

