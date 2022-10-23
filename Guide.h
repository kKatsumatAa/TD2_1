#pragma once
#include "Vector2.h"
#include "Model.h"

class Guide
{
public:
	void Initialize(Model* model, uint32_t* textureHandle_);
	void Update(Vector3 start, Vector3 end);
	void Draw(ViewProjection view);

	void SetIsLongPush(bool is) { isLongPush = is; }
	bool GetIsLongPush() { return isLongPush; }

private:
	//テクスチャハンドル
	uint32_t* textureHandle_;
	bool isLongPush = false;
	static const int Num = 50;
	WorldTransform pos_[Num];
	Vector3 playerStart_;
	Vector3 playerEnd_;
	Vector3 stageLeftTop = Vector3(-34.0f, -18.0f, 0.0f);
	Vector3 stageRightTop = Vector3(4.0f, -18.0f,0.0f);
	Vector3 stageLeftBottom = Vector3(-34.0f, 18.0f,0.0f);
	Vector3 stageRightBottom = Vector3(4.0f, 18.0f,0.0f);
	Vector3 wallStart_[4];
	Vector3 wallEnd_[4];
	Model* model_;
};