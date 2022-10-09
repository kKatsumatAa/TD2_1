#include "EnemyManager.h"
#include <random>

//��
	//�����V�[�h������
std::random_device seed_gen;
//�����Z���k�c�C�X�^�[
std::mt19937_64 engine(seed_gen());
//�����͈�
std::uniform_real_distribution<float> posDist(-20.0f, 20.0f);

void EnemyManager::Initialize(Player* player, Model* model, uint32_t* textureHandle)
{
	//////�G�����R�}���h
	//// �o�b�t�@���N���A���܂��B
	//enemyPopCommands.str("");
	//// ��Ԃ��N���A���܂��B
	//enemyPopCommands.clear(std::stringstream::goodbit);

	////�ҋ@
	//isWait = false;
	//waitTimer = 0;

	//infoTimer = 0;
	//oldInfo = false;

	//enemies.clear();
	//phase = 0;
	////phase���ς��܂ő҂t���O
	//isPhase = false;
	//isEnd[0] = { false };
	//isEnd[1] = { false };
	//isItem = false;
	//isBossDead = false;


	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	this->player = player;
	//�V���O���g���C���X�^���X���擾
	input_ = Input::GetInstance();

	//��
	for (int i = 0; i < 10; i++)
	{
		EnemyGenerate({ posDist(engine),posDist(engine),0 });
	}
}

void EnemyManager::EnemyGenerate(const Vector3& pos)
{
	//�G�𐶐��A������
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_, textureHandle_, pos);
	/*enemy->SetPlayer(player_);*/
	//�G��o�^
	enemies.push_back(std::move(enemy));
}

void EnemyManager::Update()
{
	//�G����
	enemies.remove_if([](std::unique_ptr<Enemy>& enemy)
		{
			return (enemy->GetIsDead());
		}
	);

	//��
	if (input_->TriggerKey(DIK_Z))
	{
		enemies.clear();
		for (int i = 0; i < 10; i++)
		{
			EnemyGenerate({ posDist(engine),posDist(engine),0});
		}
	}
}

void EnemyManager::Draw(const ViewProjection& view)
{
	for (std::unique_ptr<Enemy>& enemy : enemies)
	{
		enemy->Draw(view);
	}
}
