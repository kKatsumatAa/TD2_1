#pragma once
#include<sstream>

class GameSystem;

class GameSystemState
{
protected:
	GameSystem* gameSystem = nullptr;

public:
	void SetGameSystem(GameSystem* gameSystem);
	virtual void Update() = 0;
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

private:
	/// <summary>
	/// システム系コマンドの更新（すてーじ変えるstateの時のみ）
	/// </summary>
	void UpdateStageSystemCommands();

public:
	void ChangeState(GameSystemState* state);

	void initialize();

	void Update();

	int GetStage() { return stage; }
	int GetStageMax() { return stageMax; }
	int GetStageEnemyNorma() { return stageEnemyNorma; }
	int GetStageEnemyDeath() { return stageEnemyDeath; }
	int GetTime() { return time; }
	int GetBornusTime() { return bornusTime; }
	bool GetIsGameOver() { return isGameOver; }
	bool GetIsGameClear() { return isGameClear; }

	void SetStage(int stage) { this->stage = stage; }
	void SetStageEnemyNorma(int stageEnemyNorma) { this->stageEnemyNorma = stageEnemyNorma; }
	void SetStageEnemyDeath(int stageEnemyDeath) { this->stageEnemyDeath = stageEnemyDeath; }
	void SetTime(int time) { this->time = time; }
	void SetBornusTime(int bornusTime) { this->bornusTime = bornusTime; }
	void SetIsGameOver(bool isGameOver) { this->isGameOver = isGameOver; }
	void SetIsGameClear(bool isGameClear) { this->isGameClear = isGameClear; }

	void NextStage(int nowStage);

	/// <summary>
	/// システムデータの読み込み
	/// </summary>
	void LoadStageSystemData();
};


//ゲームプレイ時
class GamePlay : public GameSystemState
{
private:

public:
	void Update() override;
};

//ステージ切り替え時
class StageChange : public GameSystemState
{
private:

public:
	void Update() override;
};

//ゲームオーバー時
class GameOver : GameSystemState
{
private:

public:
	void Update() override;
};

//ゲームクリア時
class GameClear : GameSystemState
{
private:

public:
	void Update() override;
};

