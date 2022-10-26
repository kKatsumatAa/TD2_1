#include "Wall.h"
static const float PI = 3.14159265f;

void Wall::Initialize(Gravity* gravity, EffectManager* effect,Model* wall,Model* floor)
{
	gravity_ = gravity;
	effect_ = effect;

	//モデルの生成と座標の初期化
	for (int i = 0; i < 5; i++) {
		wall_[i] = Model::Create();
		wallTrans_[i].Initialize();
	}

	//角度
	for (int i = 0; i < 80; i++) {
		model_[i] = wall;
		floor_ = floor;
		modelTrans_[i].Initialize();
		if (i <= 20) {
			if (i % 2 == 0) {
				modelTrans_[i].rotation_ = { PI / 2,0,PI };
			}
			else {
				modelTrans_[i].rotation_ = { PI / 2,PI,PI };
			}
		}
		else if(i <= 40) {
			if (i % 2 == 0) {
				modelTrans_[i].rotation_ = { PI * 1.5f,PI,PI};
			}
			else {
				modelTrans_[i].rotation_ = { PI / 2,PI,PI };
			}
		}
		else if(i <= 61) {
			if (i % 2 == 0) {
				modelTrans_[i].rotation_ = { PI * 1.5f,PI,PI };
			}
			else {
				modelTrans_[i].rotation_ = { PI * 1.5f,PI * 2,PI };
			}
		}
		else {
			if (i % 2 == 0) {
				modelTrans_[i].rotation_ = { PI / 2,0,PI };
			}
			else {
				modelTrans_[i].rotation_ = { PI * 1.5f,PI * 2,PI };
			}
		}
	}

	//位置
	for (int i = 0; i < 80; i++) {
		if (i <= 20) {
			modelTrans_[i].translation_ = { (float)-35 + i * 2,19,0 };
		}
		else if (i <= 40) {
			modelTrans_[i].translation_ = { 7,(float)19 - (i - 21) * 2,0 };
		}
		else if (i <= 61) {
			modelTrans_[i].translation_ = { (float)7 - (i - 40) * 2 ,-19,0 };
		}
		else {
			modelTrans_[i].translation_ = { -35,(float) - 19 + (i - 61) * 2,0};
		}
	}


	for (int i = 0; i < 80; i++) {
		modelTrans_[i].UpdateMatrix();
	}

	//各壁の座標指定
	wallTrans_[0].translation_ = { -35,  0,  0 };
	wallTrans_[0].scale_ = { 1,20,3 };
	wallTrans_[1].translation_ = { 20,  0,  0 };
	wallTrans_[1].scale_ = { 15,20,3 };
	wallTrans_[2].translation_ = { 0,  20, 0 };
	wallTrans_[2].scale_ = { 35,1,3 };
	wallTrans_[3].translation_ = { 0,  -20,0 };
	wallTrans_[3].scale_ = { 35,1,3 };
	wallTrans_[4].translation_ = { -15,0,2 };
	wallTrans_[4].scale_ = { 21,21,0 };



	//座標をアップデート
	for (WorldTransform wallTrans : wallTrans_) {
		wallTrans.UpdateMatrix();
	}
}

void Wall::Update(GameSystem* gameSystem)
{
	if (gameSystem->GetIsStageChange())
	{
		gravity_->axis = Gravity::Axis::DOWN;
	}

	float angle = 0;

	switch (gravity_->axis)
	{
	case Gravity::Axis::DOWN:
		angle = 0;
		break;
	case Gravity::Axis::UP:
		angle = -pi;
		break;
	case Gravity::Axis::LEFT:
		angle = -pi / 2.0f;
		break;
	case Gravity::Axis::RIGHT:
		angle = pi / 2.0f;
		break;
	}
	wallTrans_[4].rotation_ = { 0,0,angle };

	//座標をアップデート
	for (WorldTransform wallTrans : wallTrans_) {
		wallTrans.UpdateMatrix();
	}
}

void Wall::Draw(ViewProjection viewProjection)
{
	for (int i = 0; i < 5; i++) {
		//wall_[i]->Draw(wallTrans_[i], viewProjection);
	}
	for (int i = 0; i < 80; i++) {
		model_[i]->Draw(modelTrans_[i], viewProjection);
	}
	floor_->Draw(wallTrans_[4], viewProjection);
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
					if (gravity_->axis != i) {
						gravity_->SetSugitaIsGomi(false);
						effect_->ShakeGenerate(30);
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
	//wallTrans_[3].scale_ = { 36,1,3 };
	wallTrans_[4].translation_.z = 5;
}