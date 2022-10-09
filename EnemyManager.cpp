#include "EnemyManager.h"
#include <random>

//仮
	//乱数シード生成器
std::random_device seed_gen;
//メルセンヌツイスター
std::mt19937_64 engine(seed_gen());
//乱数範囲
std::uniform_real_distribution<float> posDist(-20.0f, 20.0f);

void EnemyManager::Initialize(Player* player, Model* model, uint32_t* textureHandle)
{
	//////敵発生コマンド
	//// バッファをクリアします。
	//enemyPopCommands.str("");
	//// 状態をクリアします。
	//enemyPopCommands.clear(std::stringstream::goodbit);

	////待機
	//isWait = false;
	//waitTimer = 0;

	//infoTimer = 0;
	//oldInfo = false;

	//enemies.clear();
	//phase = 0;
	////phaseが変わるまで待つフラグ
	//isPhase = false;
	//isEnd[0] = { false };
	//isEnd[1] = { false };
	//isItem = false;
	//isBossDead = false;


	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	this->player = player;
	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();

	//仮
	for (int i = 0; i < 10; i++)
	{
		EnemyGenerate({ posDist(engine),posDist(engine),0 });
	}
}

void EnemyManager::EnemyGenerate(const Vector3& pos)
{
	//敵を生成、初期化
	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	enemy->Initialize(model_, textureHandle_, pos);
	/*enemy->SetPlayer(player_);*/
	//敵を登録
	enemies.push_back(std::move(enemy));
}

void EnemyManager::Update()
{
	//敵消す
	enemies.remove_if([](std::unique_ptr<Enemy>& enemy)
		{
			return (enemy->GetIsDead());
		}
	);

	//仮
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
