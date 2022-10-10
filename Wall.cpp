#include "Wall.h"

void Wall::Initialize()
{
	//���f���̐����ƍ��W�̏�����
	for (int i = 0; i < 4; i++) {
		wall_[i] = Model::Create();
		wallTrans_[i].Initialize();
	}

	//�e�ǂ̍��W�w��
	wallTrans_[0].translation_ = { -35,  0,  0 };
	wallTrans_[0].scale_ = { 1,20,1 };
	wallTrans_[1].translation_ = { 35,  0,  0 };
	wallTrans_[1].scale_ = { 1,20,1 };
	wallTrans_[2].translation_ = { 0,  20, 0 };
	wallTrans_[2].scale_ = { 36,1,1 };
	wallTrans_[3].translation_ = { 0,  -20,0 };
	wallTrans_[3].scale_ = { 36,1,1 };

	//���W���A�b�v�f�[�g
	for (WorldTransform wallTrans : wallTrans_) {
		wallTrans.UpdateMatrix();
	}
}

void Wall::Update()
{
	//���W���A�b�v�f�[�g
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

bool Wall::isCollisionWall(Vector2 pos) {
	for (WorldTransform wallTrans : wallTrans_) {
		if (wallTrans.translation_.x - wallTrans.scale_.x / 2 < pos.x && wallTrans.translation_.x + wallTrans.scale_.x / 2 > pos.x) {
			return true;
		}
		if (wallTrans.translation_.y - wallTrans.scale_.y / 2 < pos.y && wallTrans.translation_.y + wallTrans.scale_.y / 2 > pos.y) {
			return true;
		}
	}

	return false;
}