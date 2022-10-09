#pragma once
#include"Enemy.h"

class EnemyManager
{
private:
	Player* player;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	float value;
	Input* input_ = nullptr;

	////敵発生コマンド
	//std::stringstream enemyPopCommands;
	////待機
	//bool isWait = false;
	//int  waitTimer = 0;


	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	//void UpdateEnemyPopCommands();
	void EnemyGenerate(const Vector3& pos);


public:
	std::list<std::unique_ptr<Enemy>> enemies;
	//int phase = 0;
	////phaseが変わるまで待つフラグ
	//bool isPhase = false;
	//bool isEnd[2] = { false };
	//bool isItem = false;
	//bool isBossDead = false;


	void Initialize(Player* player, Model* model,  uint32_t* textureHandle);
	void Update();
	void Draw(const ViewProjection& view);

	//弾リストを取得(const参照)
	const std::list<std::unique_ptr<Enemy>>& GetEnemies()
	{
		return enemies;
	}

	/*/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();*/

	//void InfoEnd(bool& infoEnd);
};

