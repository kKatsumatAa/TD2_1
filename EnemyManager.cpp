#include "EnemyManager.h"
#include <iostream>
#include<cassert>
#include <fstream>


void EnemyManager::Initialize(Player* player, Model* model, uint32_t* textureHandle, EffectManager* effectManager, GameSystem* gameSystem,
	ItemManager* itemManager)
{
	LoadEnemyPopData();

	//�ҋ@
	isWait = false;

	enemies.clear();

	this->itemManager = itemManager;


	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	this->player = player;
	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();
	this->effectManager = effectManager;

	this->gameSystem = gameSystem;
}

void EnemyManager::EnemyGenerate(const Vector3& pos, int groupNum)
{
	//�G�𐶐��A������
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_, textureHandle_, pos, effectManager);
	enemy.get()->SetAliveNum(groupNum);
	/*enemy->SetPlayer(player_);*/
	//�G��o�^
	enemies.push_back(std::move(enemy));
}

void EnemyManager::Update()
{
	//�X�N���v�g��������
	UpdateEnemyPopCommands();

	for (std::unique_ptr<Enemy>& enemy : enemies)
	{
		//���񂾂�
		if (enemy.get()->GetIsDead())
		{
			//�G�t�F�N�g����
			effectManager->BurstGenerate(enemy.get()->GetWorldPos(), 10);
			//�|���������₷
			gameSystem->SetStageEnemyDeath(gameSystem->GetStageEnemyDeath() + 1);
		}
	}

	//�G����
	enemies.remove_if([](std::unique_ptr<Enemy>& enemy)
		{
			return (enemy->GetIsDead());
		}
	);
}

void EnemyManager::Draw(const ViewProjection& view)
{
	for (std::unique_ptr<Enemy>& enemy : enemies)
	{
		enemy->Draw(view);
	}
}

//-----------------------------------------
void EnemyManager::LoadEnemyPopData()
{
	////�G�����R�}���h
// �o�b�t�@���N���A���܂��B
	enemyPopCommands.str("");
	// ��Ԃ��N���A���܂��B
	enemyPopCommands.clear(std::stringstream::goodbit);

	//�t�@�C���J��
	std::ifstream file;
	file.open("Resources/enemyPopDatas/enemyPop.csv");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void EnemyManager::UpdateEnemyPopCommands()
{
	bool isOnaji = false;

	//1�s���̕����������ϐ�
	std::string line;
	if (!isWait)
	{
		//�R�}���h���s���[�v
		while (getline(enemyPopCommands, line))
		{
			//1�����̕�������X�g���[���ɕϊ����ĉ�͂��₷������
			std::istringstream line_stream(line);

			std::string word;
			//[,]��؂�ōs�̐擪��������擾
			getline(line_stream, word, ',');

			//[//]����n�܂�s�̓R�����g
			if (word.find("//") == 0)
			{
				continue;
			}

			if (word.find("POP") == 0)
			{
				//�O���[�v�ԍ�
				getline(line_stream, word, ',');
				int groupNum = (int)std::atoi(word.c_str());

				//x���W
				getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());

				//y���W
				getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());

				//z���W
				getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str());

				for (int i = 0; i < _countof(aliveEnemyNumber); i++)
				{
					if (aliveEnemyNumber[i] == groupNum) break;

					if (aliveEnemyNumber[i] == 0)
					{
						aliveEnemyNumber[i] = groupNum;
						groupCount++;
						break;
					}
				}

				EnemyGenerate({ x,y,z }, groupNum);

				
			}
			//ZERO�R�}���h(�G���[���ɂȂ�܂ő҂�)
			else if (word.find("ZERO") == 0)
			{
				//phase���ς��܂ő҂t���O
				isWait = true;

				//�R�}���h���[�v������
				break;//(���̍s(POP)��ǂݍ��܂Ȃ��悤��)
			}
			//ITEM�R�}���h
			else if (word.find("ITEM") == 0)
			{
				//x���W
				getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());

				//y���W
				getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());

				//z���W
				getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str());

				//scale
				getline(line_stream, word, ',');
				bool right = (float)std::atof(word.c_str());

				itemManager->ItemGenerate({ x,y,z });
			}
			//END�R�}���h
			else if (word.find("END") == 0)
			{
				//�t�@�C�����ŏ�����ǂݍ���
				LoadEnemyPopData();

				isWait = true;

				//�R�}���h���[�v������
				break;//(���̍s(POP)��ǂݍ��܂Ȃ��悤��)
			}
		}
	}

	int count[5] = { NULL };



	for (std::unique_ptr<Enemy>& enemy : enemies)
	{
		for (int i = 0; i < _countof(aliveEnemyNumber); i++)
		{
			if (enemy.get()->GetAliveNum() == aliveEnemyNumber[i] && aliveEnemyNumber[i] != 0)
			{
				if (count[i] == NULL)
				{
					count[i] = 0;
				}
				count[i]++;
			}
		}
	}

	//�O�ɋl�߂�
	for (int i = 0; i < _countof(aliveEnemyNumber); i++)
	{
		if (count[i] == 0)
		{
			if (aliveEnemyNumber[i + 1] != 0)
				aliveEnemyNumber[i] = aliveEnemyNumber[i + 1];

			if (aliveEnemyNumber[i + 2] == 0)
			{
				aliveEnemyNumber[i + 1] = 0;
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
	for (int i = 0; i < groupCount; i++)
	{
		if (count[i] == NULL && isWait)
		{

			isWait = false;
			groupCount--;

			break;
		}
	}
}