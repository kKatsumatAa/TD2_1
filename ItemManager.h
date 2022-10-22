#pragma once
#include"Item.h"

class ItemManager
{
private:
	Player* player;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	float value;
	Input* input_ = nullptr;
	HandStop* handStop = nullptr;

	EffectManager* effectManager = nullptr;

	GameSystem* gameSystem;

	////敵発生コマンド
	//std::stringstream ItemPopCommands;
	////待機
	//bool isWait = false;
	//int  waitTimer = 0;


public:
	//csv用
	int aliveItemNumber[5] = { NULL,NULL,NULL,NULL,NULL };
	int groupCountI = 0;
	std::list<std::unique_ptr<Item>> items;
	//int phase = 0;
	////phaseが変わるまで待つフラグ
	//bool isPhase = false;
	//bool isEnd[2] = { false };
	//bool isItem = false;
	//bool isBossDead = false;


	void Initialize(Player* player, Model* model, uint32_t* textureHandle, HandStop* handStop, EffectManager* effectManager, GameSystem* gameSystem);
	void Update();
	void Draw(const ViewProjection& view);

	//弾リストを取得(const参照)
	const std::list<std::unique_ptr<Item>>& GetItems()
	{
		return items;
	}

	/*/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadItemPopData();*/

	//void InfoEnd(bool& infoEnd);

		/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	//void UpdateItemPopCommands();
	void ItemGenerate(const Vector3& pos,int groupNum);
};

