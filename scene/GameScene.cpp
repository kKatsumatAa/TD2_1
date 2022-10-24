#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete wall_;
	delete set_;
	delete effectManager;
	delete sceneEffectManager;
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
	playerModel_ = Model::CreateFromOBJ("ufo", true);
	enemyModel_ = Model::CreateFromOBJ("meteorite", true);
	itemModel_ = Model::Create();
	gravityBlock_ = Model::CreateFromOBJ("gravity", true);

	titleBord_ = Model::Create();
	titleBordTrans_.Initialize();
	titleBordTrans_.translation_.y = -22;
	titleBordTrans_.scale_ = { 20,1,12 };
	titleBordTrans_.UpdateMatrix();
	titleBord2_ = Model::Create();
	titleBordTrans2_.Initialize();
	titleBordTrans2_.translation_.y = 22;
	titleBordTrans2_.scale_ = { 50,1,20 };
	titleBordTrans2_.UpdateMatrix();

	UI_back_ = Model::CreateFromOBJ("ui_back", true);
	UITrans_.Initialize();
	UITrans_.scale_ = { 15,30,1 };
	UITrans_.translation_ = { 22,0,0 };
	UITrans_.UpdateMatrix();

	effectManager = new EffectManager();
	effectManager->Initialize();
	sceneEffectManager = new SceneEffectManager();
	sceneEffectManager->Initialize();

	gameSystem.initialize(sceneEffectManager);

	gravity_ = new Gravity();
	//gravity_->Initialize(model_);
	wall_ = new Wall();
	wall_->Initialize(gravity_, effectManager);
	player_ = new Player();
	player_->Initialize(playerModel_, textureHandle_, &skillManager, &handStop, wall_, gravity_);

	enemyManager.Initialize(player_, enemyModel_, textureHandle_, effectManager, &gameSystem, &itemManager);



	skillManager.Initialize(model_, textureHandle_);

	itemManager.Initialize(player_, model_, textureHandle_, &handStop, effectManager, &gameSystem);

	grabityObj.Initialize(gravityBlock_, textureHandle_, gravity_);

	set_ = new Setting();
	set_->Initialize();

	timer_ = new Number();
	timer_->Initialize();
	nolma_ = new Number();
	nolma_->Initialize();
	kill_ = new Number();
	kill_->Initialize();
	stage_ = new Number();
	stage_->Initialize();

	timerTexture_ = TextureManager::Load("Timer.png");
	timerSprite_ = Sprite::Create(timerTexture_, { 880,250 });
	timerSprite_->SetSize({ 300, 300});
	slashTexture_ = TextureManager::Load("slash.png");
	slashSprite_ = Sprite::Create(slashTexture_, { 1010,150 });
	spaceTexture_ = TextureManager::Load("space.png");
	spaceSprite_ = Sprite::Create(spaceTexture_, { 900,600 });
	stageTexture_ = TextureManager::Load("stage.png");
	stageSprite_ = Sprite::Create(stageTexture_, { 840,40 });

	tutorial.Initialize();

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
			scene_ = Scene::Tutorial;
		}
	}


#ifdef _DEBUG
	if (isStart == false) {
		if (set_->WaitFPS() == true) {
			debugText_->SetScale(2);
			debugText_->SetPos(540, 100);
			debugText_->Printf("ARM NOBI-RU");
			debugText_->SetPos(540, 200);
			debugText_->Printf("PLESS SPACE");
			debugText_->SetScale(1);
		}
		else {
			debugText_->SetScale(2);
			debugText_->SetPos(540, 100);
			debugText_->Printf("ARM NOBI-RU");
			debugText_->SetPos(540, 200);
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
	titleBord_->Draw(titleBordTrans_, viewProjection_);
	titleBord_->Draw(titleBordTrans2_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sceneEffectManager->Draw();

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
	tutorial.Update();

	//アイテムのプロトタイプ切替
	if (input_->TriggerKey(DIK_Z))
	{
		if (colliderManager->isItemMode) colliderManager->isItemMode = false;
		else                             colliderManager->isItemMode = true;
	}

	gameSystem.Update(&tutorial);
	wall_->Update();
	enemyManager.Update();
	player_->Update(&tutorial);
	skillManager.Update();
	itemManager.Update(&tutorial);
	effectManager->Update();

	grabityObj.Update();
	sceneEffectManager->Update();


	//一番近いobjの方をplayerが向くように
	float length = NULL;
	Vector3 vec;
	std::list<Collider*> objs;
	nearObj = nullptr;
	if (!player_->GetIsRush2() && !player_->GetIsRush() && !gameSystem.GetIsStageChange())
	{
		const std::list<std::unique_ptr<Enemy>>& enemies = enemyManager.GetEnemies();
		for (const std::unique_ptr<Enemy>& enemy : enemies)
		{
			objs.push_back(enemy.get());
		}
		const std::list<std::unique_ptr<Item>>& items = itemManager.GetItems();
		for (const std::unique_ptr<Item>& item : items)
		{
			objs.push_back(item.get());
		}

		std::list<Collider*>::iterator itr = objs.begin();

		for (int i = 0; i < objs.size(); i++)
		{
			vec = ((*itr)->GetWorldPos() - player_->GetWorldPos());

			if (length > vec.GetLength() || length == NULL)
			{
				length = vec.GetLength();
				nearObj = *itr;
			}

			(*itr)->SetIsTarget(false);

			itr++;
		}

		if (nearObj != nullptr)
		{
			nearObj->SetIsTarget(true);

			vec = nearObj->GetWorldPos() - player_->GetWorldPos();

			pos = nearObj->GetWorldPos();

			angle = ((atan2(vec.y, vec.x)) - pi / 2.0f);
		}
	}
	if (player_->GetIsRush())
	{
		vec = pos - player_->GetWorldPos();

		angle = ((atan2(vec.y, vec.x)) - pi / 2.0f);
	}

	{
		player_->SetAngle(angle);
	}

	//colliderManager
	{
		colliderManager->ClearList();
		colliderManager->SetListCollider(player_);
		colliderManager->SetListCollider(&grabityObj);
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
	viewProjection_.eye = Vector3(0, 0, -50) + effectManager->ShakePow();
	viewProjection_.target = Vector3(0, 0, 0) + effectManager->ShakePow();
	viewProjection_.UpdateMatrix();


#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = Tutorial");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P) || tutorial.GetIsEnd()) {
		scene_ = Scene::MainGame;
	}

	if (input_->TriggerKey(DIK_1)) {
		sceneEffectManager->NormalSceneEffectGenerate();
	}
	if (input_->PushKey(DIK_2)) {
		sceneEffectManager->SchoolOfFishGenerate();
	}
	if (input_->TriggerKey(DIK_3)) {
		sceneEffectManager->CheckGenerate();
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
	player_->Draw(viewProjection_);
	enemyManager.Draw(viewProjection_);

	skillManager.Draw(viewProjection_);
	itemManager.Draw(viewProjection_);

	wall_->Draw(viewProjection_);
	grabityObj.Draw(viewProjection_);

	effectManager->Draw(viewProjection_);

	gameSystem.Draw();
	//gravity_->Draw(viewProjection_);

	debugText_->SetPos(10, 600);
	debugText_->Printf("アイテムのプロト:%d", colliderManager->isItemMode);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	effectManager->SpriteDraw();
	timer_->Draw({ 850,100 }, gameSystem.GetTime() / 60);
	nolma_->Draw({ 1050,300 }, gameSystem.GetStageEnemyNorma());
	kill_->Draw({ 850,300 }, gameSystem.GetStageEnemyDeath());
	sceneEffectManager->Draw();
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	tutorial.Draw();
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

	//アイテムのプロトタイプ切替
	if (input_->TriggerKey(DIK_Z))
	{
		if (colliderManager->isItemMode) colliderManager->isItemMode = false;
		else                             colliderManager->isItemMode = true;
	}

	gameSystem.Update();
	wall_->Update();
	enemyManager.Update();
	player_->Update();
	skillManager.Update();
	itemManager.Update();
	effectManager->Update();

	grabityObj.Update();
	sceneEffectManager->Update();

	//一番近いobjの方をplayerが向くように
	float length = NULL;
	Vector3 vec;
	std::list<Collider*> objs;
	nearObj = nullptr;
	if (!player_->GetIsRush2() && !player_->GetIsRush() && !gameSystem.GetIsStageChange())
	{
		const std::list<std::unique_ptr<Enemy>>& enemies = enemyManager.GetEnemies();
		for (const std::unique_ptr<Enemy>& enemy : enemies)
		{
			objs.push_back(enemy.get());
		}
		const std::list<std::unique_ptr<Item>>& items = itemManager.GetItems();
		for (const std::unique_ptr<Item>& item : items)
		{
			objs.push_back(item.get());
		}

		std::list<Collider*>::iterator itr = objs.begin();

		for (int i = 0; i < objs.size(); i++)
		{
			vec = ((*itr)->GetWorldPos() - player_->GetWorldPos());

			if (length > vec.GetLength() || length == NULL)
			{
				length = vec.GetLength();
				nearObj = *itr;
			}

			(*itr)->SetIsTarget(false);

			itr++;
		}

		if (nearObj != nullptr)
		{
			nearObj->SetIsTarget(true);

			vec = nearObj->GetWorldPos() - player_->GetWorldPos();

			pos = nearObj->GetWorldPos();

			angle = ((atan2(vec.y, vec.x)) - pi / 2.0f);
		}
	}
	if (player_->GetIsRush())
	{
		vec = pos - player_->GetWorldPos();

		angle = ((atan2(vec.y, vec.x)) - pi / 2.0f);
	}

	{
		player_->SetAngle(angle);
	}

	//colliderManager
	{
		colliderManager->ClearList();
		colliderManager->SetListCollider(player_);
		colliderManager->SetListCollider(&grabityObj);
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
	viewProjection_.eye = Vector3(0, 0, -49) + effectManager->ShakePow();
	viewProjection_.target = Vector3(0, 0, 0) + effectManager->ShakePow();
	viewProjection_.UpdateMatrix();

#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = MainGame");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P)) {
		scene_ = Scene::Gameover;
	}
	if (input_->TriggerKey(DIK_1)) {
		effectManager->BurstGenerate(Vector3(0, 0, 0), 10, 2.5f, 2.0f);
	}
	if (input_->TriggerKey(DIK_2)) {
		effectManager->ParticleGenerate(Vector3(0, 0, 0), Vector2(1000, 100));
	}
	if (input_->TriggerKey(DIK_3)) {
		sceneEffectManager->CheckGenerate();
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
	grabityObj.Draw(viewProjection_);

	effectManager->Draw(viewProjection_);

	gameSystem.Draw();

	UI_back_->Draw(UITrans_, viewProjection_);
	//gravity_->Draw(viewProjection_);

	debugText_->SetPos(10, 600);
	debugText_->Printf("アイテムのプロト:%d", colliderManager->isItemMode);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	effectManager->SpriteDraw();
	timerSprite_->Draw();
	timer_->Draw({ 985,370 }, {0,0,0,255}, gameSystem.GetTime() / 60);
	nolma_->Draw({ 1100,150 }, { 255,255,255,255 }, gameSystem.GetStageEnemyNorma());
	kill_->Draw({ 900,150 }, { 255,255,255,255 }, gameSystem.GetStageEnemyDeath());
	stage_->Draw({ 1150,40 }, { 255,255,255,255 }, gameSystem.GetStage());
	slashSprite_->Draw();
	spaceSprite_->Draw();
	stageSprite_->Draw();
	sceneEffectManager->Draw();

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

	sceneEffectManager->Update();

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
	sceneEffectManager->Draw();
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