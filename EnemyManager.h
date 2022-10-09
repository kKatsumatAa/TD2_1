#pragma once
#include"Enemy.h"

class EnemyManager
{
private:
	Player* player;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	float value;
	Input* input_ = nullptr;

	////�G�����R�}���h
	//std::stringstream enemyPopCommands;
	////�ҋ@
	//bool isWait = false;
	//int  waitTimer = 0;


	/// <summary>
	/// �G�����R�}���h�̍X�V
	/// </summary>
	//void UpdateEnemyPopCommands();
	void EnemyGenerate(const Vector3& pos);


public:
	std::list<std::unique_ptr<Enemy>> enemies;
	//int phase = 0;
	////phase���ς��܂ő҂t���O
	//bool isPhase = false;
	//bool isEnd[2] = { false };
	//bool isItem = false;
	//bool isBossDead = false;


	void Initialize(Player* player, Model* model,  uint32_t* textureHandle);
	void Update();
	void Draw(const ViewProjection& view);

	//�e���X�g���擾(const�Q��)
	const std::list<std::unique_ptr<Enemy>>& GetEnemies()
	{
		return enemies;
	}

	/*/// <summary>
	/// �G�����f�[�^�̓ǂݍ���
	/// </summary>
	void LoadEnemyPopData();*/

	//void InfoEnd(bool& infoEnd);
};

