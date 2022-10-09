#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_[0] = TextureManager::Load("kamata.ico");
	textureHandle_[1] = TextureManager::Load("uvChecker.png");
	textureHandle_[2] = TextureManager::Load("cube/cube.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	enemyManager.Initialize(player_, model_, textureHandle_);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//viewProjection_.eye = { 0,0,-100.0f };
	viewProjection_.UpdateMatrix();
}

void GameScene::Update() 
{
	player_->Update();
	enemyManager.Update();

	//colliderManager
	{
		colliderManager->ClearList();
		colliderManager->SetListCollider(player_);
		const std::list<std::unique_ptr<Enemy>>& enemies = enemyManager.GetEnemies();

		for (const std::unique_ptr<Enemy>& enemy : enemies)
		{
			colliderManager->SetListCollider(enemy.get());
		}

		colliderManager->CheckAllCollisions();

		//手との判定
		{
			colliderManager->ClearList();
			colliderManager->SetListCollider(player_->GetHandL());
			colliderManager->SetListCollider(player_->GetHandR());
			const std::list<std::unique_ptr<Enemy>>& enemies = enemyManager.GetEnemies();

			for (const std::unique_ptr<Enemy>& enemy : enemies)
			{
				colliderManager->SetListCollider(enemy.get());
			}

			colliderManager->CheckAllCollisions2();
		}

	}
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	enemyManager.Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
