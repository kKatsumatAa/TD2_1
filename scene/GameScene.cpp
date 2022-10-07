#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//プレイヤー
	player_ = Model::Create();
	playerTrans_.Initialize();
	playerTrans_.translation_ = { 0,0,0 };
	playerTexture_ = TextureManager::Load("uvChecker.png");

	//壁
	for (int i = 0; i < 4; i++) {
		wall_[i] = Model::Create();
		wallTrans_[i].Initialize();
	}

	wallTrans_[0].translation_ = { -35,  0,  0 };
	wallTrans_[0].scale_ = { 1,1,20 };
	wallTrans_[1].translation_ = { 35,  0,  0 };
	wallTrans_[1].scale_ = { 1,1,20 };
	wallTrans_[2].translation_ = { 0,  0, 20 };
	wallTrans_[2].scale_ = { 35,1,1 };
	wallTrans_[3].translation_ = { 0,  0,-20 };
	wallTrans_[3].scale_ = { 35,1,1 };

	for (WorldTransform wallTrans : wallTrans_) {
		UpdateWorldMatrix4(wallTrans);
	}

	//入力ガイド
	guide_ = Model::Create();

	guideTrans_.Initialize();
	guideTrans_.scale_ = { 0.5,0.5,0.5 };
	UpdateWorldMatrix4(guideTrans_);

	//ビュープロジェクション
	viewProjection_.Initialize();
	viewProjection_.eye = { 0,48,-1 };
	viewProjection_.target = { 0,0,0 };
	viewProjection_.UpdateMatrix();
}

void GameScene::Update() {
	if (punch == 0) {
		if (input_->PushKey(DIK_W) || input_->PushKey(DIK_S) || input_->PushKey(DIK_A) || input_->PushKey(DIK_D)) {
			if (input_->PushKey(DIK_W)) {
				guideTrans_.translation_ = playerTrans_.translation_;
				guideTrans_.translation_.z = playerTrans_.translation_.z + 1.0f;
				if (input_->PushKey(DIK_SPACE)) {
					punch = 1;
				}
			}
			if (input_->PushKey(DIK_S)) {
				guideTrans_.translation_ = playerTrans_.translation_;
				guideTrans_.translation_.z = playerTrans_.translation_.z - 1.0f;
				if (input_->PushKey(DIK_SPACE)) {
					punch = 2;
				}
			}
			if (input_->PushKey(DIK_A)) {
				guideTrans_.translation_ = playerTrans_.translation_;
				guideTrans_.translation_.x = playerTrans_.translation_.x - 1.0f;
				if (input_->PushKey(DIK_SPACE)) {
					punch = 3;
				}
			}
			if (input_->PushKey(DIK_D)) {
				guideTrans_.translation_ = playerTrans_.translation_;
				guideTrans_.translation_.x = playerTrans_.translation_.x + 1.0f;
				if (input_->PushKey(DIK_SPACE)) {
					punch = 4;
				}
			}
		}
		else {
			guideTrans_.translation_ = playerTrans_.translation_;
		}
	}
	else {
		if (guideTrans_.translation_.x > playerTrans_.translation_.x + 10.0f || guideTrans_.translation_.x < playerTrans_.translation_.x - 10.0f ||
			guideTrans_.translation_.z > playerTrans_.translation_.z + 10.0f || guideTrans_.translation_.z < playerTrans_.translation_.z - 10.0f) {
			punch = 6;
		}
		if (punch == 1) {
			guideTrans_.translation_.z += 0.1f;
			if (guideTrans_.translation_.z > 19) {
				punch = 5;
			}
		}
		else if (punch == 2) {
			guideTrans_.translation_.z -= 0.1f;
			if (guideTrans_.translation_.z < -19) {
				punch = 5;
			}
		}
		else if (punch == 3) {
			guideTrans_.translation_.x -= 0.1f;
			if (guideTrans_.translation_.x < -34) {
				punch = 5;
			}
		}
		else if (punch == 4) {
			guideTrans_.translation_.x += 0.1f;
			if (guideTrans_.translation_.x > 34) {
				punch = 5;
			}
		}
		else if (punch == 5) {
			Vector3 guideSUB = guideTrans_.translation_ - playerTrans_.translation_;
			playerTrans_.translation_ += guideSUB.GetNormalized() * 0.1f;
			if (playerTrans_.translation_.x + 1.0f >= guideTrans_.translation_.x && playerTrans_.translation_.x - 1.0f <= guideTrans_.translation_.x &&
				playerTrans_.translation_.z + 1.0f >= guideTrans_.translation_.z && playerTrans_.translation_.z - 1.0f <= guideTrans_.translation_.z) {
				punch = 0;
			}
		}
		else if (punch == 6) {
			Vector3 guideSUB = guideTrans_.translation_ - playerTrans_.translation_;
			guideTrans_.translation_ += guideSUB.GetNormalized() * -0.1f;
			if (playerTrans_.translation_.x + 1.0f >= guideTrans_.translation_.x && playerTrans_.translation_.x - 1.0f <= guideTrans_.translation_.x &&
				playerTrans_.translation_.z + 1.0f >= guideTrans_.translation_.z && playerTrans_.translation_.z - 1.0f <= guideTrans_.translation_.z) {
				punch = 0;
			}
		}
	}

	UpdateWorldMatrix4(guideTrans_);
	UpdateWorldMatrix4(playerTrans_);
}

void GameScene::Draw() {
	//すぎたきもい
	//は？
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
	for (int i = 0; i < 4; i++) {
		wall_[i]->Draw(wallTrans_[i], viewProjection_);
	}
	guide_->Draw(guideTrans_, viewProjection_);
	player_->Draw(playerTrans_, viewProjection_, playerTexture_);
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
