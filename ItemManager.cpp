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

	//��
	for (int i = 0; i < 1; i++)
	{
		ItemGenerate({ posDistX(engine),posDistY(engine),0 });
	}
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
			effectManager->ParticleGenerate(item.get()->GetWorldPos(), { 890,140 });
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

	//��
	if (input_->TriggerKey(DIK_Z) || items.size() <= 0)
	{
		items.clear();
		for (int i = 0; i < 1; i++)
		{
			ItemGenerate({ posDistX(engine),posDistY(engine),0 });
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

