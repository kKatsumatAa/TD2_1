#pragma once
#include"Item.h"

class ItemManager
{
private:
	Player* player;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	float value;
	Input* input_ = nullptr;
	HandStop* handStop = nullptr;

	EffectManager* effectManager = nullptr;

	GameSystem* gameSystem;

	////�G�����R�}���h
	//std::stringstream ItemPopCommands;
	////�ҋ@
	//bool isWait = false;
	//int  waitTimer = 0;


public:
	//csv�p
	int aliveItemNumber[5] = { NULL,NULL,NULL,NULL,NULL };
	int groupCountI = 0;
	std::list<std::unique_ptr<Item>> items;
	//int phase = 0;
	////phase���ς��܂ő҂t���O
	//bool isPhase = false;
	//bool isEnd[2] = { false };
	//bool isItem = false;
	//bool isBossDead = false;


	void Initialize(Player* player, Model* model, uint32_t* textureHandle, HandStop* handStop, EffectManager* effectManager, GameSystem* gameSystem);
	void Update();
	void Draw(const ViewProjection& view);

	//�e���X�g���擾(const�Q��)
	const std::list<std::unique_ptr<Item>>& GetItems()
	{
		return items;
	}

	/*/// <summary>
	/// �G�����f�[�^�̓ǂݍ���
	/// </summary>
	void LoadItemPopData();*/

	//void InfoEnd(bool& infoEnd);

		/// <summary>
	/// �G�����R�}���h�̍X�V
	/// </summary>
	//void UpdateItemPopCommands();
	void ItemGenerate(const Vector3& pos,int groupNum);
};

