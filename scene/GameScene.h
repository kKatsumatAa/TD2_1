﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Util.h"

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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//プレイヤー
	Model* player_ = nullptr;
	WorldTransform playerTrans_;
	uint32_t playerTexture_;

	//壁
	Model* wall_[4];
	WorldTransform wallTrans_[4];

	//入力ガイド
	Model* guide_;
	WorldTransform guideTrans_;
	
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//ロケットパンチ
	uint32_t punch;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
