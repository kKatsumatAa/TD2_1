#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include"HandSkillManager.h"

#include "EnemyManager.h"
#include "ColliderManager.h"
#include "Wall.h"
#include "Setting.h"
#include "ItemManager.h"
#include "EffectManager.h"
#include "SceneEffectManager.h"
#include "GravityObj.h"
#include "Number.h"

enum class Scene {
	Title,
	Tutorial,
	MainGame,
	Gameover,
	GameClear
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//シーン関数テーブル
	void TitleUpdateFunc();
	void TutorialUpdateFunc();
	void MainGameUpdateFunc();
	void GameoverUpdateFunc();
	void GameClearUpdateFunc();

	void TitleDrawFunc();
	void TutorialDrawFunc();
	void MainGameDrawFunc();
	void GameoverDrawFunc();
	void GameClearDrawFunc();

	static void(GameScene::* sceneUpdateFuncTable[])();
	static void(GameScene::* sceneDrawFuncTable[])();

	//スタート演出
	bool Start(float speed);

private: // メンバ変数
	//テクスチャハンドル
	uint32_t textureHandle_[10];

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	Player* player_ = nullptr;
	EnemyManager enemyManager;

	Wall* wall_ = nullptr;
	Gravity* gravity_ = nullptr;
	Setting* set_ = nullptr;
	Number* timer_ = nullptr;
	Number* nolma_ = nullptr;
	Number* kill_ = nullptr;

	EffectManager* effectManager = nullptr;
	SceneEffectManager* sceneEffectManager = nullptr;

	//衝突
	std::unique_ptr<ColliderManager> colliderManager = std::make_unique<ColliderManager>();
	//範囲こうげき
	HandSkillManager skillManager;
	//
	HandStop handStop;
	//
	ItemManager itemManager;

	//3Dモデル
	Model* model_ = nullptr;
	Model* playerModel_ = nullptr;
	Model* enemyModel_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//シーン
	Scene scene_ = Scene::Title;

	//システム
	GameSystem gameSystem;

	GravityObj grabityObj;

	//スタート演出
	bool isStart = false;

	uint32_t time = 30000;

	Collider* nearObj = nullptr;
	float angle = 0;
	Vector3 vec;
	Vector3 pos;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
