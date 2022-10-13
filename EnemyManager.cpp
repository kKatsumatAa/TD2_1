#include "EnemyManager.h"


void EnemyManager::Initialize(Player* player, Model* model, uint32_t* textureHandle, EffectManager* effectManager)
{
	//////�G�����R�}���h
	//// �o�b�t�@���N���A���܂��B
	//enemyPopCommands.str("");
	//// ��Ԃ��N���A���܂��B
	//enemyPopCommands.clear(std::stringstream::goodbit);

	////�ҋ@
	//isWait = false;
	//waitTimer = 0;

	//infoTimer = 0;
	//oldInfo = false;

	//enemies.clear();
	//phase = 0;
	////phase���ς��܂ő҂t���O
	//isPhase = false;
	//isEnd[0] = { false };
	//isEnd[1] = { false };
	//isItem = false;
	//isBossDead = false;


	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	this->player = player;
	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	this->effectManager = effectManager;

	//��
	for (int i = 0; i < 10; i++)
	{
		EnemyGenerate({ posDist(engine)-12.0f,posDist(engine),0 });
	}
}

void EnemyManager::EnemyGenerate(const Vector3& pos)
{
	//�G�𐶐��A������
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_, textureHandle_, pos, effectManager);
	/*enemy->SetPlayer(player_);*/
	//�G��o�^
	enemies.push_back(std::move(enemy));
}

void EnemyManager::Update()
{
	

	//�G��̂ɓ�̎肪���Ă�����
	for (std::unique_ptr<Enemy>& enemy : enemies)
	{
		if (enemy.get()->GetIsDead()) effectManager->BurstGenerate(enemy.get()->GetWorldPos(), 10);

	}
	//��ԋ߂��G�̕���player�������悤��
	{
		float length = NULL;
		Vector3 vec;
		Enemy* nearEnemy = nullptr;

		for (std::unique_ptr<Enemy>& enemy : enemies)
		{
			vec = (enemy.get()->GetWorldPos() - player->GetWorldPos());

			if (length > vec.GetLength() || length == NULL)
			{
				length = vec.GetLength();
				nearEnemy = enemy.get();
			}
		}

		if (nearEnemy != nullptr)
		{
			vec = nearEnemy->GetWorldPos() - player->GetWorldPos();

			player->SetAngle((atan2(vec.y, vec.x)) - pi / 2.0f);
		}
	}

	//�G����
	enemies.remove_if([](std::unique_ptr<Enemy>& enemy)
		{
			return (enemy->GetIsDead());
		}
	);

	//�G��̂ɓ�̎肪���Ă�����
	for (std::unique_ptr<Enemy>& enemy : enemies)
	{
		//state��ς���
		if (enemy.get()->GetHandCount() == 2 && !player->GetIsTwoHandOneGrab())
		{
			player->SetIsTwoHandOneGrab(true);
		}
	}

	//��
	if (input_->TriggerKey(DIK_Z) || enemies.size() <= 0)
	{
		enemies.clear();
		for (int i = 0; i < 10; i++)
		{
			EnemyGenerate({ posDist(engine)-12.0f,posDist(engine),0 });
		}
	}
}

void EnemyManager::Draw(const ViewProjection& view)
{
	for (std::unique_ptr<Enemy>& enemy : enemies)
	{
		enemy->Draw(view);
	}
}
