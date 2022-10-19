#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Util.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Vector3.h"
#include "Gravity.h"
#include "EffectManager.h"

class Wall
{
public:
	void Initialize(Gravity* gravity,EffectManager* effect);
	void Update();
	void Draw(ViewProjection viewProjection);

	Vector3 isCollisionWall(Vector3 pos, const Vector3& velocity, bool* flag = nullptr);
	void Start();

private:
	//Žl•û‚Ì•Ç
	Model* wall_[5];
	WorldTransform wallTrans_[5];
	Gravity* gravity_;
	uint32_t texture_;
	EffectManager* effect_;
};

static Vector2 stageLeftTop = Vector2(-33.0f, -18.0f);
static Vector2 stageRightTop = Vector2(3.0f, -18.0f);
static Vector2 stageLeftBottom = Vector2(-33.0f, 18.0f);
static Vector2 stageRightBottom = Vector2(3.0f, 18.0f);