#include "Wall.h"

void Wall::Initialize()
{
	//モデルの生成と座標の初期化
	for (int i = 0; i < 4; i++) {
		wall_[i] = Model::Create();
		wallTrans_[i].Initialize();
	}

	//各壁の座標指定
	wallTrans_[0].translation_ = { -35,  0,  0 };
	wallTrans_[0].scale_ = { 1,20,1 };
	wallTrans_[1].translation_ = { 35,  0,  0 };
	wallTrans_[1].scale_ = { 1,20,1 };
	wallTrans_[2].translation_ = { 0,  20, 0 };
	wallTrans_[2].scale_ = { 36,1,1 };
	wallTrans_[3].translation_ = { 0,  -20,0 };
	wallTrans_[3].scale_ = { 36,1,1 };

	//座標をアップデート
	for (WorldTransform wallTrans : wallTrans_) {
		wallTrans.UpdateMatrix();
	}
}

void Wall::Update()
{
	//座標をアップデート
	for (WorldTransform wallTrans : wallTrans_) {
		wallTrans.UpdateMatrix();
	}
}

void Wall::Draw(ViewProjection viewProjection)
{
	for (int i = 0; i < 4; i++) {
		wall_[i]->Draw(wallTrans_[i], viewProjection);
	}
}

Vector3 Wall::isCollisionWall(Vector3 pos, const Vector3& velocity,bool* flag) {
	for (int i = 0; i < 100; i++) {
		
		for (WorldTransform wallTrans : wallTrans_) {
			if (wallTrans.translation_.x - wallTrans.scale_.x < pos.x + 1 && wallTrans.translation_.x + wallTrans.scale_.x > pos.x - 1) {
				if (wallTrans.translation_.y - wallTrans.scale_.y < pos.y + 1 && wallTrans.translation_.y + wallTrans.scale_.y > pos.y - 1) {
					pos += velocity * -0.01f;
					if (flag != nullptr) {
						*flag = false;
					}
 					return pos;
				}
			}
		}
		pos += velocity * 0.01f;
	}

	return pos;
}