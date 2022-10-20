#pragma once
#include"Enemy.h"
#include"GameSystem.h"
#include"ItemManager.h"

class EnemyManager
{
private:
	Player* player;
	GameSystem* gameSystem;
	ItemManager* itemManager;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	float value;
	Input* input_ = nullptr;

	EffectManager* effectManager = nullptr;

	//�o�����Ă���G�̔ԍ���ۊ�
	int aliveEnemyNumber[5] = { NULL,NULL,NULL,NULL,NULL };


	//�G�����R�}���h
	std::stringstream enemyPopCommands;
	//�ҋ@
	bool isWait = false;
	int groupCount = 0;


	/// <summary>
	/// �G�����R�}���h�̍X�V
	/// </summary>
	void UpdateEnemyPopCommands();
	void EnemyGenerate(const Vector3& pos,int groupNum);


public:
	std::list<std::unique_ptr<Enemy>> enemies;
	
	void Initialize(Player* player, Model* model,  uint32_t* textureHandle,
		EffectManager* effectManager, GameSystem* gameSystem, ItemManager* itemManager);
	void Update();
	void Draw(const ViewProjection& view);

	//�e���X�g���擾(const�Q��)
	const std::list<std::unique_ptr<Enemy>>& GetEnemies()
	{
		return enemies;
	}

	/// <summary>
	/// �G�����f�[�^�̓ǂݍ���
	/// </summary>
	void LoadEnemyPopData();
};

