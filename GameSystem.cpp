#include "GameSystem.h"
#include <iostream>
#include<cassert>
#include <fstream>


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
			time = (int)std::atoi(word.c_str());

			//�m���}���擾
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
	//�t�@�C���J��
	std::ifstream file;
	file.open("Resources/stageDatas/stageTimeNormaData.csv");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	stageSystemCommands << file.rdbuf();

	//�t�@�C�������
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
