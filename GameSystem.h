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

	DebugText* debugText_;
	SceneEffectManager* sceneEffect_;
	Sprite* sprite_;

	//�O���Ŏg�p���邽�߂̃X�e�[�W�ύX�����t���O
	bool isStageChange = false;

public:
	//�V�[���J�ڂ̎���
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
	/// �V�X�e���f�[�^�̓ǂݍ���
	/// </summary>
	void LoadStageSystemData();
	/// <summary>
	/// �V�X�e���n�R�}���h�̍X�V�i���ā[���ς���state�̎��̂݁j
	/// </summary>
	void UpdateStageSystemCommands();
};


//�Q�[���v���C��
class GamePlay : public GameSystemState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
	void Draw() override;
};

//�X�e�[�W�؂�ւ���
class StageChange : public GameSystemState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
	void Draw() override;
};

//�Q�[���I�[�o�[��
class GameOver : public GameSystemState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
	void Draw() override;
};

//�Q�[���N���A��
class GameClear : public GameSystemState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
	void Draw() override;
};

