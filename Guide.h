#pragma once
#include "Vector2.h"
#include "Model.h"

class Guide
{
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection view);

private:
	WorldTransform pos_;
	Vector3 playerStart_;
	Vector3 playerEnd_;
	Vector3 stageLeftTop = Vector3(-33.0f, -18.0f, 0.0f);
	Vector3 stageRightTop = Vector3(3.0f, -18.0f,0.0f);
	Vector3 stageLeftBottom = Vector3(-33.0f, 18.0f,0.0f);
	Vector3 stageRightBottom = Vector3(3.0f, 18.0f,0.0f);
	Vector3 WallStart_[4];
	Vector3 WallEnd_[4];
	Model* model_;
};

static Vector2 stageLeftTop = Vector2(-33.0f, -18.0f);
static Vector2 stageRightTop = Vector2(3.0f, -18.0f);
static Vector2 stageLeftBottom = Vector2(-33.0f, 18.0f);
static Vector2 stageRightBottom = Vector2(3.0f, 18.0f);