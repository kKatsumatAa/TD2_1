#pragma once
#include"PlayerHand.h"


class Player;

class PlayerHandState
{
protected:
	Player* player = nullptr;

public:
	void SetPlayer(Player* player);
	virtual void Update() = 0;
};


class Player
{
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform worldTransformHand_;
	//モデル
	Model* model_ = nullptr;
	Model* modelHand_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//手の最大の長さ
	const float handLengthMax = 10.0f;
	//角度
	float angle = 0.0f;

	//手の状態
	PlayerHandState* state = nullptr;
	Vector2 handVecR;
	Vector2 handVecL;
	
	


public:
	void ChangeState(PlayerHandState* state);

	void Initialize(Model* model, const uint32_t textureHandle);
	void Update();
	void Draw(const ViewProjection& view);
	//手を伸ばす
	void ReachOut();

	Vector3 GetWorldPos();
};

//片手で１つをつかんでいる状態
class OneHandOneGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};

//両手で１つをつかんでいる状態
class TwoHandOneGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};

//両手で２つをつかんでいる状態
class TwoHandTwoGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};
