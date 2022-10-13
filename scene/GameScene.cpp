#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete wall_;
	delete set_;
	delete effect_;
}

void (GameScene::* GameScene::sceneUpdateFuncTable[])() = {
	&GameScene::TitleUpdateFunc,
	&GameScene::TutorialUpdateFunc,
	&GameScene::MainGameUpdateFunc,
	&GameScene::GameoverUpdateFunc,
	&GameScene::GameClearUpdateFunc
};

void (GameScene::* GameScene::sceneDrawFuncTable[])() = {
	&GameScene::TitleDrawFunc,
	&GameScene::TutorialDrawFunc,
	&GameScene::MainGameDrawFunc,
	&GameScene::GameoverDrawFunc,
	&GameScene::GameClearDrawFunc
};

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_[0] = TextureManager::Load("kamata.ico");
	textureHandle_[1] = TextureManager::Load("uvChecker.png");
	textureHandle_[2] = TextureManager::Load("cube/cube.jpg");
	textureHandle_[3] = TextureManager::Load("axis/axis.jpg");
	textureHandle_[4] = TextureManager::Load("sample.png");

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.eye = { 0,-49,-1 };
	viewProjection_.UpdateMatrix();

	//3Dモデルの生成
	model_ = Model::Create();

	effect_ = new EffectManager();
	effect_->Initialize(viewProjection_);

	wall_ = new Wall();
	wall_->Initialize();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &skillManager, &handStop, wall_);

	enemyManager.Initialize(player_, model_, textureHandle_, effect_);


	skillManager.Initialize(model_, textureHandle_);

	itemManager.Initialize(player_, model_, textureHandle_, &handStop);

	set_ = new Setting();
	set_->Initialize();


	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	
}

void GameScene::Update()
{
	//下の方にシーンごとのプロトタイプ関数があるので\
	そっちにプログラムを書いてください
	(this->*sceneUpdateFuncTable[static_cast<size_t>(scene_)])();

	//シーンに関係なく描画するもの
	set_->FPS(60.0f);
	set_->DrawFPS();
}

void GameScene::Draw() {
	//下の方にシーンごとのプロトタイプ関数があるので\
	そっちにプログラムを書いてください
	(this->*sceneDrawFuncTable[static_cast<size_t>(scene_)])();
}

//関数テーブル(プロトタイプ)
#pragma region タイトルシーン
/// <summary>
/// タイトルアップデート
/// </summary>
void GameScene::TitleUpdateFunc() {
	if (set_->WaitFPS() == true) {
		if (input_->TriggerKey(DIK_SPACE)) {
			isStart = true;
		}
	}
	if (isStart == true) {
		if (Start(0.4f) == true) {
			wall_->Start();
			scene_ = Scene::MainGame;
		}
	}


#ifdef _DEBUG
	if (isStart == false) {
		if (set_->WaitFPS() == true) {
			debugText_->SetPos(540, 200);
			debugText_->SetScale(2);
			debugText_->Printf("PLESS SPACE");
			debugText_->SetScale(1);
		}
		else {
			debugText_->SetPos(540, 200);
			debugText_->SetScale(2);
			debugText_->Printf("NOW LOADING");
			debugText_->SetScale(1);
		}
	}
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = Title");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P)) {
		scene_ = Scene::Tutorial;
		wall_->Start();
		viewProjection_.eye = { 0,0,-50 };
		viewProjection_.UpdateMatrix();
	}
#endif
}
/// <summary>
/// タイトル描画
/// </summary>
void GameScene::TitleDrawFunc() {
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
	wall_->Draw(viewProjection_);
	player_->Draw(viewProjection_);

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
#pragma endregion 

#pragma region チュートリアル
/// <summary>
/// チュートリアルアップデート
/// </summary>
void GameScene::TutorialUpdateFunc() {

	effect_->Update();




#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = Tutorial");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P)) {
		scene_ = Scene::MainGame;
	}

	if (input_->TriggerKey(DIK_1)) {
		effect_->BurstGenerate(Vector3(0, 0, 0), 5);
	}
#endif
}

/// <summary>
/// チュートリアル描画
/// </summary>
void GameScene::TutorialDrawFunc() {
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
	effect_->Draw();

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
#pragma endregion

#pragma region メインゲーム
/// <summary>
/// メインゲームアップデート
/// </summary>
void GameScene::MainGameUpdateFunc() {

	wall_->Update();
	player_->Update();
	enemyManager.Update();
	skillManager.Update();
	itemManager.Update();
	effect_->Update();

	//colliderManager
	{
		colliderManager->ClearList();
		colliderManager->SetListCollider(player_);
		const std::list<std::unique_ptr<Enemy>>& enemies = enemyManager.GetEnemies();
		for (const std::unique_ptr<Enemy>& enemy : enemies)
		{
			colliderManager->SetListCollider(enemy.get());
		}
		const std::list<std::unique_ptr<HandSkill>>& skills = skillManager.GetSkills();
		for (const std::unique_ptr<HandSkill>& skill : skills)
		{
			colliderManager->SetListCollider(skill.get());
		}
		const std::list<std::unique_ptr<Item>>& items = itemManager.GetItems();
		for (const std::unique_ptr<Item>& item : items)
		{
			colliderManager->SetListCollider(item.get());
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
			const std::list<std::unique_ptr<Item>>& items = itemManager.GetItems();
			for (const std::unique_ptr<Item>& item : items)
			{
				colliderManager->SetListCollider(item.get());
			}

			colliderManager->CheckAllCollisions2();
		}

	}

#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = MainGame");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P)) {
		scene_ = Scene::Gameover;
	}
	if (input_->TriggerKey(DIK_1)) {
		effect_->BurstGenerate(Vector3(0, 0, 0), 10, 2.5f, 2.0f);
	}
#endif
}
/// <summary>
/// メインゲーム描画
/// </summary>
void GameScene::MainGameDrawFunc() {
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

	skillManager.Draw(viewProjection_);
	itemManager.Draw(viewProjection_);

	wall_->Draw(viewProjection_);

	effect_->Draw();


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
#pragma endregion

#pragma region ゲームオーバー
/// <summary>
/// ゲームオーバーアップデート
/// </summary>
void GameScene::GameoverUpdateFunc() {

#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = GameOver");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P)) {
		scene_ = Scene::GameClear;
	}
#endif
}
/// <summary>
/// ゲームオーバー描画
/// </summary>
void GameScene::GameoverDrawFunc() {
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
#pragma endregion

#pragma region ゲームクリア
/// <summary>
/// ゲームクリアアップデート
/// </summary>
void GameScene::GameClearUpdateFunc() {

#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = GameClear");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P)) {
		scene_ = Scene::Title;
	}
#endif
}
/// <summary>
/// ゲームクリア描画
/// </summary>
void GameScene::GameClearDrawFunc() {
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
#pragma endregion

//プロトタイプ関数
bool GameScene::Start(float speed) {
	if (viewProjection_.eye.y >= 0) {
		return true;
	}
	else {
		viewProjection_.eye.y += speed;
		viewProjection_.eye.z -= speed;
		viewProjection_.UpdateMatrix();
	}

	return false;
}