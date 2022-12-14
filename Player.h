#pragma once
#include"PlayerHand.h"
#include"HandSkillManager.h"
#include"Gravity.h"
#include "Guide.h"
#include "Tutorial.h"
#include "Audio.h"

class Player;

class PlayerHandState
{
protected:
	Player* player = nullptr;

public:
	void SetPlayer(Player* player);
	virtual void Update(Tutorial* tutorial = nullptr) = 0;
};


class Player : public Collider
{
private:
	WorldTransform worldTransformHand_;
	WorldTransform worldTransformHand2_;
	//モデル
	Model* model_ = nullptr;
	Model* modelGuide_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	DebugText* debugText_ = nullptr;
	HandSkillManager* skillManager;
	

	Wall* wall = nullptr;

	Vector3 velocity;

	//角度
	float angle = 0.0f;

	//手の状態
	PlayerHandState* state = nullptr;
	PlayerHand handR;

	//貫通攻撃かどうか
	bool isRush = false;
	bool isRush2 = false;

	
public:
	//sound
	uint32_t* soundDataHandle = nullptr;
	uint32_t* voiceHandle = nullptr;
	Audio* audio = nullptr;
	Input* input_ = nullptr;
	Gravity* gravity;
	Guide* guide = nullptr;
	HandStop* handStop = nullptr;

	bool isEffect = false;

	void ChangeState(PlayerHandState* state);

	void Initialize(Model* model, Model* guideM, uint32_t* textureHandle, HandSkillManager* skillManager, HandStop* handStop, Wall* wall, Gravity* gravity,
		Audio* audio,uint32_t * soundDataHandle,uint32_t* voiceHandle);
	void Update(GameSystem* gameSystem, bool isEffect,Tutorial* tutorial = nullptr);
	void Draw(const ViewProjection& view);
	//手を伸ばす
	void ReachOut();

	Vector3 GetWorldPos() override;
	void SetWorldPos(const Vector3& pos) { worldTransform_.translation_ = pos; };
	PlayerHand* GetHandR() { return &handR; }
	float GetAngle() { return worldTransform_.rotation_.z; }
	HandSkillManager* GetSkillManager() { return skillManager; }

	void SetVelocity(Vector3 vec) { velocity = vec; }
	Vector3 GetVelocity() { return velocity; }
	Wall* GetWall() { return wall; }
	Guide* GetGuid() { return guide; }

	bool GetIsRush() { return isRush; }
	void SetIsRush(bool is) { isRush = is; }
	bool GetIsRush2() { return isRush2; }
	void SetIsRush2(bool is) { isRush2 = is; }
	//衝突を検出したら呼び出す（コールバック関数）
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;
};


//何もしていない
class NoGrab : public PlayerHandState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
};

//片手で１つをつかんでいる状態
class OneHandOneGrab : public PlayerHandState
{
private:
	bool isNotHandRushAttack = false;

public:
	void Update(Tutorial* tutorial = nullptr) override;
};

//普通の攻撃している状態
class OneHandAttack : public PlayerHandState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
};

//貫通攻撃の準備状態
class OneHandRushGrab : public PlayerHandState
{
private:


public:
	void Update(Tutorial* tutorial = nullptr) override;
};
//貫通攻撃している状態
class OneHandRushAttack : public PlayerHandState
{
private:

public:
	void Update(Tutorial* tutorial = nullptr) override;
};
//貫通攻撃している状態2
class OneHandRushAttack2 : public PlayerHandState
{
private:
	int timer = 0;
	const int maxTimer = 30;

public:
	void Update(Tutorial* tutorial = nullptr) override;
};