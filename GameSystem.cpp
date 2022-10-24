#include "GameSystem.h"
#include <iostream>
#include<cassert>
#include <fstream>

void GameSystem::initialize(SceneEffectManager* sceneEffect)
{
	debugText_ = DebugText::GetInstance();
	sceneEffect_ = sceneEffect;
	//���݂̃X�e�[�W�����Ԗڂ�
	stage = 0;
	//�X�e�[�W�؂�ւ����Ƀm���}������
	stageEnemyNorma = 0;
	//���̃X�e�[�W�łǂ񂾂��|������
	stageEnemyDeath = 0;
	//�c�莞��
	time = 0;
	//�O�X�e�[�W����̌J��z���̎���
	bornusTime = 0;

	isGameOver = false;
	isGameClear = false;

	//�O���Ŏg�p���邽�߂̃X�e�[�W�ύX�����t���O
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
	debugText_->SetPos(500, 30);
	debugText_->Printf("Stage:%d Time:%d kill:%d norma:%d bornusTime:%d", stage, time / 60, stageEnemyDeath, stageEnemyNorma, bornusTime);

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
	//�Ō�̃X�e�[�W��������
	if (gameSystem->GetStage() >= gameSystem->GetStageMax())
	{
		gameSystem->ChangeState(new GameClear);
	}
	else
	{
		//������Sleep�Ƃ������ăV�[���J�ڂ̉��o�Ƃ��G�̃f�[�^�̓ǂݍ��݂Ƃ������Ă���������
		//

		//�R�}���h�f�[�^�ǂݍ���
		gameSystem->UpdateStageSystemCommands();

		//�������ԂɃ{�[�i�X�^�C����ǉ�
		gameSystem->SetTime(gameSystem->GetTime() + gameSystem->GetBornusTime());
		gameSystem->SetBornusTime(0);

		//�|�����������Z�b�g
		gameSystem->SetStageEnemyDeath(0);

		//�X�e�[�W�i�߂�
		gameSystem->NextStage(gameSystem->GetStage());

		//�X�e�[�W�ς������t���O
		gameSystem->SetIsStageChange(true);

		//�X�e�[�g���Q�[���ɕς���
		gameSystem->ChangeState(new GamePlay);
	}
}

void StageChange::Draw()
{
}

//-------------------------------------------------------------
void GamePlay::Update(Tutorial* tutorial)
{
	//���Ԍ��炷
	if (gameSystem->GetTime() > 0) {
		gameSystem->SetTime(gameSystem->GetTime() - 1);
	}

	if (tutorial == nullptr)
	{
		//�m���}�B��������
		if (gameSystem->GetStageEnemyDeath() >= gameSystem->GetStageEnemyNorma())
		{
			if (gameSystem->GetSceneTime() == gameSystem->SCENE_TIME) {
				gameSystem->GetSceneEffect()->NormalSceneEffectGenerate();
			}
			gameSystem->SubSceneTime();
			if (gameSystem->GetSceneTime() < 0) {
				gameSystem->ResetSceneTime();
				gameSystem->ChangeState(new StageChange);
			}
		}
		//�������ԏI�������
		else if (gameSystem->GetTime() <= 0)
		{
			if (gameSystem->GetSceneTime() == gameSystem->SCENE_TIME) {
				gameSystem->GetSceneEffect()->CheckGenerate();
			}
			gameSystem->SubSceneTime();
			if (gameSystem->GetSceneTime() < 0) {
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
	debugText_->SetPos(500, 45);
	debugText_->Printf("GAME_OVER");
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
	debugText_->SetPos(500, 45);
	debugText_->Printf("GAME_CLEAR");
}

//----------------------------------------------------------------
void GameSystem::UpdateStageSystemCommands()
{
	//1�s���̕����������ϐ�
	std::string line;

	//�R�}���h���s���[�v
	while (getline(stageSystemCommands, line))
	{
		//1�����̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//[,]��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');

		//[//]����n�܂�s�̓R�����g
		if (word.find("//") == 0)
		{
			continue;
		}
		//stage�R�}���h
		else if (word.find("S") == 0)
		{
			//�������Ԃ��擾
			getline(line_stream, word, ',');
			time = (int)std::atoi(word.c_str()) * 60;

			//�m���}���擾
			getline(line_stream, word, ',');
			stageEnemyNorma = (float)std::atof(word.c_str());

			break;
		}
	}
}

void GameSystem::LoadStageSystemData()
{
	////�G�����R�}���h
// �o�b�t�@���N���A���܂��B
	stageSystemCommands.str("");
	// ��Ԃ��N���A���܂��B
	stageSystemCommands.clear(std::stringstream::goodbit);
	//�t�@�C���J��
	std::ifstream file;
	file.open("Resources/stageDatas/stageTimeNormaData.txt");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	stageSystemCommands << file.rdbuf();

	//�t�@�C�������
	file.close();
}