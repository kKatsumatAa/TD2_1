#include "GameSystem.h"
#include <iostream>
#include<cassert>
#include <fstream>


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
			time = (int)std::atoi(word.c_str());

			//ノルマを取得
			getline(line_stream, word, ',');
			stageEnemyNorma = (float)std::atof(word.c_str());

			break;
		}
	}
}

void GameSystem::ChangeState(GameSystemState* state)
{
	delete this->state;
	this->state = state;
	state->SetGameSystem(this);
}

void GameSystem::initialize()
{
	state = new StageChange;
	state->SetGameSystem(this);
}

void GameSystem::Update()
{
	state->Update();
}


void GameSystem::NextStage(int nowStage)
{
	SetStage(nowStage + 1);
}

void GameSystem::LoadStageSystemData()
{
	//ファイル開く
	std::ifstream file;
	file.open("Resources/stageDatas/stageTimeNormaData.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	stageSystemCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}


//-------------------------------------------------------------
void GameSystemState::SetGameSystem(GameSystem* gameSystem)
{
	this->gameSystem = gameSystem;
}

//-------------------------------------------------------------
void GamePlay::Update()
{
}

//-------------------------------------------------------------
void StageChange::Update()
{
}

//-------------------------------------------------------------
void GameOver::Update()
{
}

//-------------------------------------------------------------
void GameClear::Update()
{
}
