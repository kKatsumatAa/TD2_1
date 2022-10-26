#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::DeleteGameScene() {
	SafeDelete(wall_);
	SafeDelete(set_);
	SafeDelete(effectManager);
	SafeDelete(sceneEffectManager);
	SafeDelete(player_);
	SafeDelete(gravity_);
	SafeDelete(timer_);
	SafeDelete(nolma_);
	SafeDelete(kill_);
	SafeDelete(stage_);
}

void GameScene::ResetGameScene(bool isTutorial) {
	SafeDelete(effectManager);
	SafeDelete(sceneEffectManager);
	SafeDelete(player_);
	SafeDelete(gravity_);
	SafeDelete(timer_);
	SafeDelete(nolma_);
	SafeDelete(kill_);
	SafeDelete(stage_);


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.eye = { 0,-49,-40 };
	viewProjection_.target = { 0,0,-39 };
	viewProjection_.UpdateMatrix();

	playerTrans_.Initialize();
	playerTrans_.translation_ = { 0,50,-50 };
	playerTrans_.scale_ = { 3,3,3 };
	playerTrans_.rotation_ = { 0,0,0 };
	playerTrans_.UpdateMatrix();

	isStart = false;
	ufo_ = 0;
	//3Dモデルの生成
	effectManager = new EffectManager();
	effectManager->Initialize(textureHandle_);
	sceneEffectManager = new SceneEffectManager();
	sceneEffectManager->Initialize(textureHandle_);

	gameSystem.initialize(sceneEffectManager,&handStop);

	gravity_ = new Gravity();
	//gravity_->Initialize(model_);
	wall_ = new Wall();
	wall_->Initialize(gravity_, effectManager, wallModel_, floorModel_);
	player_ = new Player();
	player_->Initialize(playerModel_, aimModel_, textureHandle_, &skillManager, &handStop, wall_, gravity_);

	if (isTutorial)
		enemyManager.Initialize(player_, enemyModel_, textureHandle_, effectManager, &gameSystem, &itemManager, &tutorial);
	else
		enemyManager.Initialize(player_, enemyModel_, textureHandle_, effectManager, &gameSystem, &itemManager);

	skillManager.Initialize(model_, textureHandle_);

	itemManager.Initialize(player_, model_, textureHandle_, &handStop, effectManager, &gameSystem);

	grabityObj.Initialize(gravityBlock_, textureHandle_, gravity_);

	timer_ = new Number();
	timer_->Initialize(textureHandle_[10]);
	nolma_ = new Number();
	nolma_->Initialize(textureHandle_[10]);
	kill_ = new Number();
	kill_->Initialize(textureHandle_[10]);
	stage_ = new Number();
	stage_->Initialize(textureHandle_[10]);

	tutorial.Initialize();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
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
	textureHandle_[1] = TextureManager::Load("meteorite_2.png");
	textureHandle_[2] = TextureManager::Load("cube/cube.jpg");
	textureHandle_[3] = TextureManager::Load("axis/axis.jpg");
	textureHandle_[4] = TextureManager::Load("sample.png");
	textureHandle_[5] = TextureManager::Load("particle.png");
	textureHandle_[6] = TextureManager::Load("meteorite_2.png");
	textureHandle_[7] = TextureManager::Load("nextStage.png");
	textureHandle_[8] = TextureManager::Load("timeover.png");
	textureHandle_[9] = TextureManager::Load("dotline2.png");
	textureHandle_[10] = TextureManager::Load("number.png");
	textureHandle_[12] = TextureManager::Load("stop.png");
	textureHandle_[11] = TextureManager::Load("back/back.png");

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.eye = { 0,-49,-40 };
	viewProjection_.target = { 0,0,-39 };
	viewProjection_.UpdateMatrix();

	//3Dモデルの生成
	model_ = Model::Create();
	playerModel_ = Model::CreateFromOBJ("ufo_", true);
	playerTrans_.Initialize();
	playerTrans_.translation_ = { 0,50,-50 };
	playerTrans_.scale_ = { 3,3,3 };
	playerTrans_.UpdateMatrix();

	enemyModel_ = Model::CreateFromOBJ("meteorite", true);

	gravityBlock_ = Model::CreateFromOBJ("gravity", true);
	aimModel_ = Model::CreateFromOBJ("aim", true);
	wallModel_ = Model::CreateFromOBJ("wall_3", true);
	floorModel_ = Model::CreateFromOBJ("floor", true);

	titleModel_ = Model::CreateFromOBJ("title", true);
	titleTrans_.Initialize();
	titleTrans_.translation_ = {0,200,-50};
	titleTrans_.scale_ = { 7,7,7 };
	titleTrans_.rotation_ = {pi / 2,pi,0};
	titleTrans_.UpdateMatrix();

	UI_back_ = Model::CreateFromOBJ("ui_back", true);
	UITrans_.Initialize();
	UITrans_.scale_ = { 15,20,1 };
	UITrans_.translation_ = { 22,0,1 };
	UITrans_.UpdateMatrix();

	itemModel_ = Model::CreateFromOBJ("item", true);

	backModel_ = Model::CreateFromOBJ("back", true);
	backTrans_.Initialize();
	backTrans_.scale_ = { 100,100,100 };
	backTrans_.UpdateMatrix();


	effectManager = new EffectManager();
	effectManager->Initialize(textureHandle_);
	sceneEffectManager = new SceneEffectManager();
	sceneEffectManager->Initialize(textureHandle_);

	gameSystem.initialize(sceneEffectManager, &handStop);

	gravity_ = new Gravity();
	//gravity_->Initialize(model_);
	wall_ = new Wall();
	wall_->Initialize(gravity_, effectManager, wallModel_, floorModel_);
	player_ = new Player();
	player_->Initialize(playerModel_,aimModel_, textureHandle_, &skillManager, &handStop, wall_, gravity_);

	enemyManager.Initialize(player_, enemyModel_, textureHandle_, effectManager, &gameSystem, &itemManager);

	skillManager.Initialize(model_, textureHandle_);

	itemManager.Initialize(player_, itemModel_, textureHandle_, &handStop, effectManager, &gameSystem);

	grabityObj.Initialize(gravityBlock_, textureHandle_, gravity_);

	set_ = new Setting();
	set_->Initialize();

	timer_ = new Number();
	timer_->Initialize(textureHandle_[10]);
	nolma_ = new Number();
	nolma_->Initialize(textureHandle_[10]);
	kill_ = new Number();
	kill_->Initialize(textureHandle_[10]);
	stage_ = new Number();
	stage_->Initialize(textureHandle_[10]);
	bonus_ = new Number();
	bonus_->Initialize(textureHandle_[10]);

	timerTexture_ = TextureManager::Load("Timer.png");
	timerSprite_ = Sprite::Create(timerTexture_, { 830,250 });
	timerSprite_->SetSize({ 300, 300 });
	timerTexture2_ = TextureManager::Load("Timer.png");
	timerSprite2_ = Sprite::Create(timerTexture_, { 1130,430 });
	timerSprite2_->SetColor({255,255,0,255});
	slashTexture_ = TextureManager::Load("slash.png");
	slashSprite_ = Sprite::Create(slashTexture_, { 1010,150 });
	spaceTexture_ = TextureManager::Load("space.png");
	spaceSprite_ = Sprite::Create(spaceTexture_, { 900,600 });
	stageTexture_ = TextureManager::Load("stage.png");
	stageSprite_ = Sprite::Create(stageTexture_, { 840,40 });

	stopSprite_ = Sprite::Create(textureHandle_[12], { 0,0 });

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
			ResetGameScene(true);
		}
	}

	playerTrans_.rotation_.z += 0.1f;
	ufo_ += 0.03f;
	playerTrans_.translation_.z += sin(ufo_) * 0.4f;
	playerTrans_.translation_.x += cos(ufo_) * 0.8f;
	playerTrans_.UpdateMatrix();

	titleTrans_.UpdateMatrix();

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
		ResetGameScene(true);
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
	UI_back_->Draw(UITrans_, viewProjection_);
	backModel_->Draw(backTrans_, viewProjection_,textureHandle_[11]);
	wall_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	titleModel_->Draw(titleTrans_, viewProjection_);
	playerModel_->Draw(playerTrans_, viewProjection_);
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
	//spaceSprite_->Draw();
	// デバッグテキストの描画
	//debugText_->DrawAll(commandList);
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
	if (input_->TriggerKey(DIK_1))
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

	if (tutorial.GetIsEnd()) {
		scene_ = Scene::MainGame;
		ResetGameScene();
	}
#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = Tutorial");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P) || tutorial.GetIsEnd()) {
		scene_ = Scene::MainGame;
		ResetGameScene();
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
	
	backModel_->Draw(backTrans_, viewProjection_,textureHandle_[11]);
	enemyManager.Draw(viewProjection_);

	skillManager.Draw(viewProjection_);
	itemManager.Draw(viewProjection_);

	wall_->Draw(viewProjection_);
	grabityObj.Draw(viewProjection_);

	effectManager->Draw(viewProjection_);

	gameSystem.Draw();

	UI_back_->Draw(UITrans_, viewProjection_);
	player_->Draw(viewProjection_);
	player_->guide->Draw(viewProjection_);
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
	timerSprite2_->Draw();
	timer_->Draw({ 960,370 }, { 0,0,0,255 }, gameSystem.GetTime() / 60);
	nolma_->Draw({ 1100,150 }, { 255,255,255,255 }, gameSystem.GetStageEnemyNorma());
	kill_->Draw({ 900,150 }, { 255,255,255,255 }, gameSystem.GetStageEnemyDeath());
	stage_->Draw({ 1150,40 }, { 255,255,255,255 }, gameSystem.GetStage());
	bonus_->Draw({ 1175,465 }, { 0,0,0,255 }, gameSystem.GetBornusTime(),14);
	slashSprite_->Draw();
	//spaceSprite_->Draw();
	stageSprite_->Draw();
	itemManager.DrawSprite();
	sceneEffectManager->Draw();
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	tutorial.Draw();
	if (handStop.GetIsStop())stopSprite_->Draw();
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
	if (input_->TriggerKey(DIK_1))
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


	if (gameSystem.GetIsGameClear()) {
		scene_ = Scene::GameClear;
		playerTrans_.translation_ = { 0,0,0 };
		playerTrans_.rotation_ = { pi / 2,0,0 };
		playerTrans_.UpdateMatrix();
		//ResetGameScene();
	}
	else if (gameSystem.GetIsGameOver()) {
		scene_ = Scene::Gameover;
		playerTrans_.translation_ = { 30,20,0 };
		playerTrans_.rotation_ = { pi / 2,0,0 };
		playerTrans_.UpdateMatrix();
		//ResetGameScene();
	}

#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = MainGame");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P)) {
		scene_ = Scene::Gameover;
		playerTrans_.translation_ = { 30,20,0 };
		playerTrans_.rotation_ = { pi / 2,0,0 };
		playerTrans_.UpdateMatrix();
	}
	if (input_->TriggerKey(DIK_1)) {
		effectManager->BurstGenerate(Vector3(0, 0, 0), 10, 2.5f, 2.0f);
	}
	if (input_->TriggerKey(DIK_2)) {
		effectManager->ParticleGenerate(Vector3(-20, 20, 0), Vector2(1000, 100));
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
	backModel_->Draw(backTrans_, viewProjection_, textureHandle_[11]);
	enemyManager.Draw(viewProjection_);

	skillManager.Draw(viewProjection_);
	itemManager.Draw(viewProjection_);

	wall_->Draw(viewProjection_);
	
	grabityObj.Draw(viewProjection_);

	effectManager->Draw(viewProjection_);

	gameSystem.Draw();

	UI_back_->Draw(UITrans_, viewProjection_);
	player_->Draw(viewProjection_);
	player_->guide->Draw(viewProjection_);
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
	//player_->guide->DrawSprite();
	effectManager->SpriteDraw();
	timerSprite_->Draw();
	timerSprite2_->Draw();
	timer_->Draw({ 960,370 }, { 0,0,0,255 }, gameSystem.GetTime() / 60);
	nolma_->Draw({ 1100,150 }, { 255,255,255,255 }, gameSystem.GetStageEnemyNorma());
	kill_->Draw({ 900,150 }, { 255,255,255,255 }, gameSystem.GetStageEnemyDeath());
	stage_->Draw({ 1150,40 }, { 255,255,255,255 }, gameSystem.GetStage());
	bonus_->Draw({ 1175,465 }, { 0,0,0,255 }, gameSystem.GetBornusTime(), 14);
	slashSprite_->Draw();
	spaceSprite_->Draw();
	stageSprite_->Draw();
	itemManager.DrawSprite();
	sceneEffectManager->Draw();

	if (handStop.GetIsStop())stopSprite_->Draw();
	// デバッグテキストの描画
	//debugText_->DrawAll(commandList);
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
	playerTrans_.translation_ += {-0.5f,-0.3f,0};
	playerTrans_.rotation_ += {0.3f,0.1f,0.2f};
	playerTrans_.UpdateMatrix();

#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = GameOver");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P)) {
		scene_ = Scene::GameClear;
		playerTrans_.translation_ = { 0,0,0 };
		playerTrans_.rotation_ = { pi / 2,0,0 };
		playerTrans_.UpdateMatrix();
	}

#endif

	if (input_->TriggerKey(DIK_SPACE))
	{
		scene_ = Scene::Title;
		ResetGameScene();
	}
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
	
	backModel_->Draw(backTrans_, viewProjection_, textureHandle_[11]);
	playerModel_->Draw(playerTrans_, viewProjection_);

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

	sceneEffectManager->Update();
	playerTrans_.rotation_.z += 0.1f;
	ufo_ += 0.03f;
	playerTrans_.translation_.z += sin(ufo_) * 0.4f;
	playerTrans_.translation_.x += cos(ufo_) * 0.8f;
	playerTrans_.UpdateMatrix();

#ifdef _DEBUG
	debugText_->SetPos(1100, 20);
	debugText_->Printf("Scene = GameClear");
	debugText_->SetPos(1100, 40);
	debugText_->Printf("[P] = NextScene");
	if (input_->TriggerKey(DIK_P)) {
		scene_ = Scene::Title;
	}
#endif

	if (input_->TriggerKey(DIK_SPACE))
	{
		scene_ = Scene::Title;
		ResetGameScene();
	}
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
	
	backModel_->Draw(backTrans_, viewProjection_, textureHandle_[11]);
	playerModel_->Draw(playerTrans_, viewProjection_);

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

//プロトタイプ関数
bool GameScene::Start(float speed) {
	if (viewProjection_.eye.y >= 0) {
		viewProjection_.eye.z += speed;
		viewProjection_.UpdateMatrix();
		if (viewProjection_.eye.z >= -49) {
			return true;
		}
	}
	else {
		viewProjection_.eye.y += speed;
		viewProjection_.eye.z -= speed;
		viewProjection_.UpdateMatrix();
	}

	return false;
}