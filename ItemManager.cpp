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
	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	this->handStop = handStop;
}

void ItemManager::ItemGenerate(const Vector3& pos)
{
	//�G�𐶐��A������
	std::unique_ptr<Item> item = std::make_unique<Item>();
	item->Initialize(model_, textureHandle_, pos, handStop, effectManager);
	/*item->SetPlayer(player_);*/
	//�G��o�^
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
	//�j�󂳂ꂽ��
	for (std::unique_ptr<Item>& item : items)
	{
		if (item.get()->GetIsDead())
		{
			//�G�t�F�N�g
			effectManager->ParticleGenerate(item.get()->GetWorldPos(), { 1000,10 });
			//�{�[�i�X�^�C���ǉ�
			gameSystem->SetBornusTime(gameSystem->GetBornusTime() + item.get()->GetBonusTime());
		}
	}
	//����
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

