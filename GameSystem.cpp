#include "GameSystem.h"
#include <iostream>
#include<cassert>
#include <fstream>

void GameSystem::initialize(SceneEffectManager* sceneEffect, HandStop* handStop)
{
	debugText_ = DebugText::GetInstance();
	sceneEffect_ = sceneEffect;
	//現在のステージが何番目か
	stage = 0;
	//ステージ切り替え時にノルマを入れる
	stageEnemyNorma = 0;
	//今のステージでどんだけ倒したか
	stageEnemyDeath = 0;
	//残り時間
	time = 0;
	//前ステージからの繰り越しの時間
	bornusTime = 0;

	this->handStop = handStop;

	isGameOver = false;
	isGameClear = false;

	//外部で使用するためのステージ変更したフラグ
	isStageChange = false;

	sceneTime = SCENE_TIME;

	LoadStageSystemData();

	state = new StageChange;
	state->SetGameSystem(this);
	state->Update();
}

void GameSystem::ChangeState(GameSystemState* state)
{
	delete this->state;
	this->state = state;
	state->SetGameSystem(this);
}

void GameSystem::Update(Tutorial* tutorial)
{
	state->Update(tutorial);
}

void GameSystem::Draw()
{

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
void StageChange::Update(Tutorial* tutorial)
{
	//最後のステージだったら
	if (gameSystem->GetStage() >= gameSystem->GetStageMax())
	{
		if (gameSystem->GetSceneTime() == gameSystem->SCENE_TIME) {
			gameSystem->GetSceneEffect()->CheckGenerate(13);
		}
		gameSystem->SubSceneTime();
		if (gameSystem->GetSceneTime() < 150) {
			gameSystem->ResetSceneTime();
			gameSystem->ChangeState(new GameClear);
		}
	}
	else
	{
		//ここでSleepとかさせてシーン遷移の演出とか敵のデータの読み込みとかさせてもいいかも
		//

		//コマンドデータ読み込み
		gameSystem->UpdateStageSystemCommands();

		//制限時間にボーナスタイムを追加
		gameSystem->SetTime(gameSystem->GetTime() + gameSystem->GetBornusTime() * 60);
		gameSystem->SetBornusTime(0);

		//倒した数をリセット
		gameSystem->SetStageEnemyDeath(0);

		//ステージ進める
		gameSystem->NextStage(gameSystem->GetStage());

		//ステージ変わったよフラグ
		gameSystem->SetIsStageChange(true);

		//ステートをゲームに変える
		gameSystem->ChangeState(new GamePlay);
	}
}

void StageChange::Draw()
{
}

//-------------------------------------------------------------
void GamePlay::Update(Tutorial* tutorial)
{
	if (gameSystem->GetIsStageChange())
	{
		gameSystem->SetIsStageChange(false);
	}

	//時間減らす
	if (gameSystem->GetTime() > 0) {
		if (gameSystem->handStop->GetIsStop())
			gameSystem->SetTime(gameSystem->GetTime() - 0.2f);
		else
			gameSystem->SetTime(gameSystem->GetTime() - 1.0f);
	}

	if (tutorial == nullptr)
	{
		//ノルマ達成したら
		if (gameSystem->GetStageEnemyDeath() >= gameSystem->GetStageEnemyNorma())
		{
			if (gameSystem->GetSceneTime() == gameSystem->SCENE_TIME) {
				if (gameSystem->GetStage() < gameSystem->GetStageMax() - 1) {
 					gameSystem->GetSceneEffect()->NormalSceneEffectGenerate(7);
				}
				else if(gameSystem->GetStage() < gameSystem->GetStageMax()) {
					gameSystem->GetSceneEffect()->NormalSceneEffectGenerate(14);
				}
			}
			gameSystem->SubSceneTime();
			if (gameSystem->GetSceneTime() < 0) {
				gameSystem->ResetSceneTime();
				gameSystem->ChangeState(new StageChange);
			}
		}
		//制限時間終わったら
		else if (gameSystem->GetTime() <= 0)
		{
			if (gameSystem->GetSceneTime() == gameSystem->SCENE_TIME) {
				gameSystem->GetSceneEffect()->CheckGenerate(8);
			}
			gameSystem->SubSceneTime();
			if (gameSystem->GetSceneTime() < 150) {
				gameSystem->ResetSceneTime();
				gameSystem->ChangeState(new GameOver);
			}
		}
	}
}

void GamePlay::Draw()
{
}

//-------------------------------------------------------------
void GameOver::Update(Tutorial* tutorial)
{
	gameSystem->SubSceneTime();
	if (gameSystem->GetSceneTime() <= 0) {
		gameSystem->SetIsGameOver(true);
	}
}

void GameOver::Draw()
{
}

//-------------------------------------------------------------
void GameClear::Update(Tutorial* tutorial)
{
	gameSystem->SubSceneTime();
	if (gameSystem->GetSceneTime() <= 0) {
		gameSystem->SetIsGameClear(true);

	}
}

void GameClear::Draw()
{
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
	////敵発生コマンド
// バッファをクリアします。
	stageSystemCommands.str("");
	// 状態をクリアします。
	stageSystemCommands.clear(std::stringstream::goodbit);
	//ファイル開く
	std::ifstream file;
	file.open("Resources/stageDatas/stageTimeNormaData.txt");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	stageSystemCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}