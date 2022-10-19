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

		//�X�e�[�g���Q�[���ɕς���
		gameSystem->ChangeState(new GamePlay);
	}
}

void StageChange::Draw()
{
}

//-------------------------------------------------------------
void GamePlay::Update()
{
	//���Ԍ��炷
	gameSystem->SetTime(gameSystem->GetTime() - 1);

	//�m���}�B��������
	if (gameSystem->GetStageEnemyDeath() >= gameSystem->GetStageEnemyNorma())
	{
		gameSystem->ChangeState(new StageChange);
	}
	//�������ԏI�������
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
	//�t�@�C���J��
	std::ifstream file;
	file.open("Resources/stageDatas/stageTimeNormaData.txt");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	stageSystemCommands << file.rdbuf();

	//�t�@�C�������
	file.close();
}