#pragma once
#include "Vector2.h"
#include "Model.h"

class Guide
{
public:
	void Initialize(Model* model);
	void Update(Vector3 start, Vector3 end);
	void Draw(ViewProjection view);

private:
	static const int Num = 60;
	WorldTransform pos_[Num];
	Vector3 playerStart_;
	Vector3 playerEnd_;
	Vector3 stageLeftTop = Vector3(-32.0f, -18.0f, 0.0f);
	Vector3 stageRightTop = Vector3(3.0f, -18.0f,0.0f);
	Vector3 stageLeftBottom = Vector3(-32.0f, 18.0f,0.0f);
	Vector3 stageRightBottom = Vector3(3.0f, 18.0f,0.0f);
	Vector3 wallStart_[4];
	Vector3 wallEnd_[4];
	Model* model_;
};