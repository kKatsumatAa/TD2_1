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
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	float value;
	Input* input_ = nullptr;

	EffectManager* effectManager = nullptr;

	//出現している敵の番号を保管
	int aliveEnemyNumber[5] = { NULL,NULL,NULL,NULL,NULL };


	//敵発生コマンド
	std::stringstream enemyPopCommands;
	//待機
	bool isWait = false;
	int groupCount = 0;


	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();
	void EnemyGenerate(const Vector3& pos,int groupNum);


public:
	std::list<std::unique_ptr<Enemy>> enemies;
	
	void Initialize(Player* player, Model* model,  uint32_t* textureHandle,
		EffectManager* effectManager, GameSystem* gameSystem, ItemManager* itemManager);
	void Update();
	void Draw(const ViewProjection& view);

	//弾リストを取得(const参照)
	const std::list<std::unique_ptr<Enemy>>& GetEnemies()
	{
		return enemies;
	}

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();
};

