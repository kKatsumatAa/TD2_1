#include "GameSystem.h"
#include <iostream>
#include<cassert>
#include <fstream>

void GameSystem::initialize()
{
	debugText_ = DebugText::GetInstance();

	LoadStageSystemData();

	state = new StageChange;
	state->SetGameSystem(this);
}

void GameSystem::ChangeState(GameSystemState* state)
{
	delete this->state;
	this->state = state;
	state->SetGameSystem(this);
}

void GameSystem::Update()
{
	state->Update();
}

void GameSystem::Draw()
{
	debugText_->SetPos(500, 30);
	debugText_->Printf("Stage:%d Time:%d kill:%d norma:%d bornusTime:%d",stage, time / 60, stageEnemyDeath, stageEnemyNorma, bornusTime);

	state->Draw();
}


void GameSystem::NextStage(int nowStage)
{
	SetStage(nowStage + 1);
}


//-------------------------------------------------------------
void GameSystemState::SetGameSystem(GameSystem* gameSystem)
{
	this->gameSystem = gameSystem;
}

//-------------------------------------------------------------
void StageChange::Update()
{
	//最後のステージだったら
	if (gameSystem->GetStage() >= gameSystem->GetStageMax())
	{
		gameSystem->ChangeState(new GameClear);
	}
	else
	{
		//ここでSleepとかさせてシーン遷移の演出とか敵のデータの読み込みとかさせてもいいかも
		//

		//コマンドデータ読み込み
		gameSystem->UpdateStageSystemCommands();

		//制限時間にボーナスタイムを追加
		gameSystem->SetTime(gameSystem->GetTime() + gameSystem->GetBornusTime());
		gameSystem->SetBornusTime(0);

		//倒した数をリセット
		gameSystem->SetStageEnemyDeath(0);

		//ステージ進める
		gameSystem->NextStage(gameSystem->GetStage());

		//ステートをゲームに変える
		gameSystem->ChangeState(new GamePlay);
	}
}

void StageChange::Draw()
{
}

//-------------------------------------------------------------
void GamePlay::Update()
{
	//時間減らす
	gameSystem->SetTime(gameSystem->GetTime() - 1);

	//ノルマ達成したら
	if (gameSystem->GetStageEnemyDeath() >= gameSystem->GetStageEnemyNorma())
	{
		gameSystem->ChangeState(new StageChange);
	}
	//制限時間終わったら
	else if (gameSystem->GetTime() <= 0)
	{
		gameSystem->ChangeState(new GameOver);
	}
}

void GamePlay::Draw()
{
}

//-------------------------------------------------------------
void GameOver::Update()
{
	gameSystem->SetIsGameOver(true);
}

void GameOver::Draw()
{
	debugText_->SetPos(500, 45);
	debugText_->Printf("GAME_OVER");
}

//-------------------------------------------------------------
void GameClear::Update()
{
	gameSystem->SetIsGameClear(true);
}

void GameClear::Draw()
{
	debugText_->SetPos(500, 45);
	debugText_->Printf("GAME_CLEAR");
}

//----------------------------------------------------------------
void GameSystem::UpdateStageSystemCommands()
{
	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(stageSystemCommands, line))
	{
		//1桁分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//[,]区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//[//]から始まる行はコメント
		if (word.find("//") == 0)
		{
			continue;
		}
		//stageコマンド
		else if (word.find("S") == 0)
		{
			//制限時間を取得
			getline(line_stream, word, ',');
			time = (int)std::atoi(word.c_str()) * 60;

			//ノルマを取得
			getline(line_stream, word, ',');
			stageEnemyNorma = (float)std::atof(word.c_str());

			break;
		}
	}
}

void GameSystem::LoadStageSystemData()
{
	//ファイル開く
	std::ifstream file;
	file.open("Resources/stageDatas/stageTimeNormaData.txt");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	stageSystemCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}