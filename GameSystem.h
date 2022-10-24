#pragma once
#include<sstream>
#include"DebugText.h"
#include"SceneEffectManager.h"
#include "Sprite.h"
#include "Tutorial.h"

class GameSystem;

class GameSystemState
{
protected:
	GameSystem* gameSystem = nullptr;
	DebugText* debugText_ = DebugText::GetInstance();

public:
	void SetGameSystem(GameSystem* gameSystem);
	virtual void Update(Tutorial* tutorial = nullptr) = 0;
	virtual void Draw() = 0;
};


class GameSystem
{
private:
	//現在のステージが何番目か
	int stage = 0;
	//ステージの最大数
	static const int stageMax = 5;
	//ステージ切り替え時にノルマを入れる
	int stageEnemyNorma = 0;
	//今のステージでどんだけ倒したか
	int stageEnemyDeath = 0;
	//残り時間
	int time = 0;
	//前ステージからの繰り越しの時間
	int bornusTime = 0;

	bool isGameOver = false;
	bool isGameClear = false;

	//手の状態
	GameSystemState* state = nullptr;

	//システムのコマンド
	std::stringstream stageSystemCommands;
	std::string word;

	DebugText* debugText_;
	SceneEffectManager* sceneEffect_;
	Sprite* sprite_;

	//外部で使用するためのステージ変更したフラグ
	bool isStageChange = false;

public:
	//シーン遷移の時間
	static const int SCENE_TIME = 150;
	int sceneTime = SCENE_TIME;
private:


public:
	void ChangeState(GameSystemState* state);

	void initialize(SceneEffectManager* sceneEffect);

	void Update(Tutorial* tutorial = nullptr);
	void Draw();

	int GetStage() { return stage; }
	int GetStageMax() { return stageMax; }
	int GetStageEnemyNorma() { return stageEnemyNorma; }
	int GetStageEnemyDeath() { return stageEnemyDeath; }
	int GetTime() { return time; }
	int GetBornusTime() { return bornusTime; }
	bool GetIsGameOver() { return isGameOver; }
	bool GetIsGameClear() { return isGameClear; }
	bool GetIsStageChange() { return isStageChange; }
	int GetSceneTime() {
		return sceneTime;
	}
	SceneEffectManager* GetSceneEffect() {
		return sceneEffect_;
	}

	void SetStage(int stage) { this->stage = stage; }
	void SetStageEnemyNorma(int stageEnemyNorma) { this->stageEnemyNorma = stageEnemyNorma; }
	void SetStageEnemyDeath(int stageEnemyDeath) { this->stageEnemyDeath = stageEnemyDeath; }
	void SetTime(int time) { this->time = time; }
	void SetBornusTime(int bornusTime) { this->bornusTime = bornusTime; }
	void SetIsGameOver(bool isGameOver) { this->isGameOver = isGameOver; }
	void SetIsGameClear(bool isGameClear) { this->isGameClear = isGameClear; }
	void SetIsStageChange(bool is) { isStageChange = is; }
	void ResetSceneTime() {
		sceneTime = SCENE_TIME;
	}

	void SubSceneTime() {
		sceneTime--;
	}

	void NextStage(int nowStage);

	/// <summary>
	/// システムデータの読み込み
	/// </summary>
	void LoadStageSystemData();
	/// <summary>
	/// システム系コマンドの更新（すてーじ変えるstateの時のみ）
	/// </summary>
	void UpdateStageSystemCommands();
};


//ゲームプレイ時
class GamePlay : public GameSystemState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
	void Draw() override;
};

//ステージ切り替え時
class StageChange : public GameSystemState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
	void Draw() override;
};

//ゲームオーバー時
class GameOver : public GameSystemState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
	void Draw() override;
};

//ゲームクリア時
class GameClear : public GameSystemState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
	void Draw() override;
};

