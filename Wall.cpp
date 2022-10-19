#include "Wall.h"

void Wall::Initialize(Gravity* gravity)
{
	gravity_ = gravity;

	//モデルの生成と座標の初期化
	for (int i = 0; i < 5; i++) {
		wall_[i] = Model::Create();
		wallTrans_[i].Initialize();
	}
	texture_ = TextureManager::Load("white1x1.png");

	//各壁の座標指定
	wallTrans_[0].translation_ = { -35,  0,  0 };
	wallTrans_[0].scale_ = { 1,20,3 };
	wallTrans_[1].translation_ = { 20,  0,  0 };
	wallTrans_[1].scale_ = { 15,20,3 };
	wallTrans_[2].translation_ = { 0,  20, 0 };
	wallTrans_[2].scale_ = { 35,1,3 };
	wallTrans_[3].translation_ = { 0,  -20,0 };
	wallTrans_[3].scale_ = { 35,1,3 };
	wallTrans_[4].translation_ = { 0,0,0 };
	wallTrans_[4].scale_ = { 40,20,0 };

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
	for (int i = 0; i < 5; i++) {

		wall_[i]->Draw(wallTrans_[i], viewProjection/*, texture_*/);
	}
}

Vector3 Wall::isCollisionWall(Vector3 pos, const Vector3& velocity, bool* flag) {
	for (int i = 0; i < 100; i++) {

		for (int i = 0; i < 4; i++) {
			if (wallTrans_[i].translation_.x - wallTrans_[i].scale_.x < pos.x + 1 && wallTrans_[i].translation_.x + wallTrans_[i].scale_.x > pos.x - 1) {
				if (wallTrans_[i].translation_.y - wallTrans_[i].scale_.y < pos.y + 1 && wallTrans_[i].translation_.y + wallTrans_[i].scale_.y > pos.y - 1) {
					pos += velocity * -0.01f;
					if (flag != nullptr) {
						*flag = true;
					}
					gravity_->axis = i;
					return pos;
				}
			}
		}
		pos += velocity * 0.01f;
	}

	return pos;
}

void Wall::Start() {
	wallTrans_[3].scale_ = { 36,1,3 };
	wallTrans_[4].translation_.z = 0;
}