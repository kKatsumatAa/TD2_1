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
	//���݂̃X�e�[�W�����Ԗڂ�
	int stage = 0;
	//�X�e�[�W�̍ő吔
	static const int stageMax = 5;
	//�X�e�[�W�؂�ւ����Ƀm���}������
	int stageEnemyNorma = 0;
	//���̃X�e�[�W�łǂ񂾂��|������
	int stageEnemyDeath = 0;
	//�c�莞��
	int time = 0;
	//�O�X�e�[�W����̌J��z���̎���
	int bornusTime = 0;

	bool isGameOver = false;
	bool isGameClear = false;

	//��̏��
	GameSystemState* state = nullptr;

	//�V�X�e���̃R�}���h
	std::stringstream stageSystemCommands;
	std::string word;

private:
	/// <summary>
	/// �V�X�e���n�R�}���h�̍X�V�i���ā[���ς���state�̎��̂݁j
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
	/// �V�X�e���f�[�^�̓ǂݍ���
	/// </summary>
	void LoadStageSystemData();
};


//�Q�[���v���C��
class GamePlay : public GameSystemState
{
private:

public:
	void Update() override;
};

//�X�e�[�W�؂�ւ���
class StageChange : public GameSystemState
{
private:

public:
	void Update() override;
};

//�Q�[���I�[�o�[��
class GameOver : GameSystemState
{
private:

public:
	void Update() override;
};

//�Q�[���N���A��
class GameClear : GameSystemState
{
private:

public:
	void Update() override;
};

