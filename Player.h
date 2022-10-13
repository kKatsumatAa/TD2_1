#pragma once
#include"PlayerHand.h"
#include"HandSkillManager.h"
#include"Gravity.h"


class Player;

class PlayerHandState
{
protected:
	Player* player = nullptr;

public:
	void SetPlayer(Player* player);
	virtual void Update() = 0;
};


class Player : public Collider
{
private:
	WorldTransform worldTransformHand_;
	//モデル
	Model* model_ = nullptr;
	Model* modelHand_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;
	HandSkillManager* skillManager;
	HandStop* handStop;
	Wall* wall = nullptr;
	

	Vector3 velocity;

	//角度
	float angle = 0.0f;

	//手の状態
	PlayerHandState* state = nullptr;
	PlayerHand handR;
	PlayerHand handL;
	//使用中の手を順に入れる用
	PlayerHand* useHands[2];

	//一体の敵を二つの手でつかんでいるか
	bool isTwoHandOneGrab = false;

public:
	int useHandCount = 0;
	Input* input_ = nullptr;
	Gravity* gravity;

	void ChangeState(PlayerHandState* state);

	void Initialize(Model* model, uint32_t* textureHandle, HandSkillManager* skillManager, HandStop* handStop, Wall* wall, Gravity* gravity);
	void Update();
	void Draw(const ViewProjection& view);
	//手を伸ばす
	void ReachOut();

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };
	PlayerHand* GetHandR() { return &handR; }
	PlayerHand* GetHandL() { return &handL; }
	PlayerHand** GetUseHands() { return useHands; }
	float GetAngle() { return worldTransform_.rotation_.z; }
	HandSkillManager* GetSkillManager() { return skillManager; }
	HandStop* GetHandStop() { return handStop; }

	void SetIsTwoHandOneGrab(const bool& is) { isTwoHandOneGrab = is; }
	bool GetIsTwoHandOneGrab() { return isTwoHandOneGrab; }

	void SetVelocity(Vector3 vec) { velocity = vec; }
	Vector3 GetVelocity() { return velocity; }
	Wall* GetWall() {return wall;}
	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision()override;
	void OnCollision2(Collider& collider)override;
};


//何もしていない
class NoGrab : public PlayerHandState
{
private:

public:
	void Update() override;
};

//片手で１つをつかんでいる状態（小さい範囲こうげき）
class OneHandOneGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};

//両手を使っている
class TwoHand : public PlayerHandState
{
private:

public:
	void Update() override;
};

//両手で１つをつかんでいる状態（突進の直線状の範囲こうげき）
class TwoHandOneGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};
class TwoHandOneGrab2 : public PlayerHandState
{
private:
	int timer = 0;
	const int maxTimer = 15;

public:
	void Update() override;
};

//両手で２つをつかんでいる状態（先に延ばしてたやつを倒したら次のやつに突進）
class TwoHandTwoGrab : public PlayerHandState
{
private:


public:
	void Update() override;
};
