#include "EnemyManager.h"
#include <iostream>
#include<cassert>
#include <fstream>


void EnemyManager::Initialize(Player* player, Model* model, uint32_t* textureHandle, EffectManager* effectManager, GameSystem* gameSystem,
	ItemManager* itemManager)
{
	this->gameSystem = gameSystem;

	LoadEnemyPopData();

	//待機
	isWait = false;

	enemies.clear();

	this->itemManager = itemManager;


	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	this->player = player;
	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
	this->effectManager = effectManager;
}

void EnemyManager::EnemyGenerate(const Vector3& pos, int groupNum)
{
	//敵を生成、初期化
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_, textureHandle_, pos, effectManager);
	enemy.get()->SetAliveNum(groupNum);
	/*enemy->SetPlayer(player_);*/
	//敵を登録
	enemies.push_back(std::move(enemy));
}

void EnemyManager::Update()
{
	

	//スクリプト発生処理
	UpdateEnemyPopCommands();

	for (std::unique_ptr<Enemy>& enemy : enemies)
	{
		//死んだら
		if (enemy.get()->GetIsDead())
		{
			//エフェクト発生
			effectManager->BurstGenerate(enemy.get()->GetWorldPos(), 10);
			//倒した数増やす
			gameSystem->SetStageEnemyDeath(gameSystem->GetStageEnemyDeath() + 1);
		}
	}

	//敵消す
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
	////敵発生コマンド
// バッファをクリアします。
	enemyPopCommands.str("");
	// 状態をクリアします。
	enemyPopCommands.clear(std::stringstream::goodbit);

	//ファイル開く
	std::ifstream file;
	switch (gameSystem->GetStage())
	{
	case 1:
  		file.open("Resources/enemyPopDatas/enemyPop1.csv");
		break;
	case 2:
 		file.open("Resources/enemyPopDatas/enemyPop2.csv");
		break;
	case 3:
		file.open("Resources/enemyPopDatas/enemyPop3.csv");
		break;
	case 4:
		file.open("Resources/enemyPopDatas/enemyPop4.csv");
		break;
	case 5:
		file.open("Resources/enemyPopDatas/enemyPop5.csv");
		break;
	}
	
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void EnemyManager::UpdateEnemyPopCommands()
{
	
	//ステージ変わったら敵消す
	if (gameSystem->GetIsStageChange())
	{
		//待機
		isWait = false;
		groupCount = 0;
		LoadEnemyPopData();
		gameSystem->SetIsStageChange(false);
		enemies.clear();
		aliveEnemyNumber[0] = 0;
		aliveEnemyNumber[1] = 0;
		aliveEnemyNumber[2] = 0;
		aliveEnemyNumber[3] = 0;
		aliveEnemyNumber[4] = 0;
	}

	bool isOnaji[2] = { false,false };

	//1行分の文字列を入れる変数
	std::string line;
	if (!isWait)
	{
		//コマンド実行ループ
		while (getline(enemyPopCommands, line))
		{
			//1桁分の文字列をストリームに変換して解析しやすくする
			std::istringstream line_stream(line);

			std::string word;
			//[,]区切りで行の先頭文字列を取得
			getline(line_stream, word, ',');

			//[//]から始まる行はコメント
			if (word.find("//") == 0)
			{
				isOnaji[0] = false;
				isOnaji[1] = false;
				continue;
			}

			if (word.find("POP") == 0)
			{
				//グループ番号
				getline(line_stream, word, ',');
				int groupNum = (int)std::atoi(word.c_str());

				//x座標
				getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());

				//y座標
				getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());

				//z座標
				getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str());

				for (int i = 0; i < _countof(aliveEnemyNumber); i++)
				{
					if (aliveEnemyNumber[i] == groupNum && !isOnaji[0]) isOnaji[1] = true;
					if (isOnaji[1]) break;

					if (aliveEnemyNumber[i] == 0 && !isOnaji[0])
					{
						aliveEnemyNumber[i] = groupNum;
						groupCount++;
						break;
					}
				}

				if(!isOnaji[1]) EnemyGenerate({x,y,z}, groupNum);

				isOnaji[0] = true;
			}
			//ZEROコマンド(敵がゼロになるまで待つ)
			else if (word.find("ZERO") == 0)
			{
				//phaseが変わるまで待つフラグ
				isWait = true;

				//コマンドループ抜ける
				break;//(次の行(POP)を読み込まないように)
			}
			//ITEMコマンド
			else if (word.find("ITEM") == 0)
			{
				//x座標
				getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());

				//y座標
				getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());

				//z座標
				getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str());

				//scale
				getline(line_stream, word, ',');
				bool right = (float)std::atof(word.c_str());

				itemManager->ItemGenerate({ x,y,z });
			}
			//ENDコマンド
			else if (word.find("END") == 0)
			{
				//ファイルを最初から読み込む
				LoadEnemyPopData();

				isWait = true;

				//コマンドループ抜ける
				break;//(次の行(POP)を読み込まないように)
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

	//前に詰める
	for (int i = 0; i < _countof(aliveEnemyNumber)-2; i++)
	{
		if (count[i] == 0)
		{
			if (aliveEnemyNumber[i + 1] != 0)
				aliveEnemyNumber[i] = aliveEnemyNumber[i + 1];
			else aliveEnemyNumber[i] = 0;

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