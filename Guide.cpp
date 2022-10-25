#include "Guide.h"

void Guide::Initialize(Model* model, uint32_t* textureHandle_)
{
	this->textureHandle_ = textureHandle_;
	model_ = model;
	texture_ = textureHandle_[9];

	for (int i = 0; i < Num; i++) {
		pos_[i].Initialize();
		pos_[i].rotation_ = { 0,pi / 2,0 };
	}
	wallStart_[0] = stageLeftTop;
	wallEnd_[0] = stageRightTop;
	wallStart_[1] = stageLeftTop;
	wallEnd_[1] = stageLeftBottom;
	wallStart_[2] = stageRightTop;
	wallEnd_[2] = stageRightBottom;
	wallStart_[3] = stageLeftBottom;
	wallEnd_[3] = stageRightBottom;
}

void Guide::Update(Vector3 start, Vector3 end)
{
	for (int i = 0; i < Num; i++) {
		textNum_[i] = 0;
	}
	for (int i = 0; i < Num; i++) {
		start += end;
		pos_[i].translation_ = start;
		pos_[i].scale_ = { 0.1f,0.1f,0.1f };
		pos_[i].UpdateMatrix();
		if (pos_[i].translation_.x <= wallStart_[0].x) {
			pos_[i].scale_ = { 0.1f,1.0f,1.0f };
			pos_[i].translation_.x = wallStart_[0].x;
			textNum_[i] = 1;
			pos_[i].UpdateMatrix();
			for (int j = i + 1; j < Num; j++) {
				pos_[j].translation_ = { -100,-100,-100 };
				pos_[j].UpdateMatrix();
			}
			break;
		}
		else if (pos_[i].translation_.x >= wallEnd_[0].x) {
			pos_[i].scale_ = { 0.1f,1.0f,1.0f };
			pos_[i].translation_.x = wallEnd_[0].x;
			textNum_[i] = 1;
			pos_[i].UpdateMatrix();
			for (int j = i + 1; j < Num; j++) {
				pos_[j].translation_ = { -100,-100,-100 };
				pos_[j].UpdateMatrix();
			}
			break;
		}
		else if (pos_[i].translation_.y <= wallStart_[0].y) {
			pos_[i].scale_ = { 0.1f,1.0f,1.0f };
			pos_[i].translation_.y = wallStart_[0].y;
			textNum_[i] = 1;
			pos_[i].UpdateMatrix();
			for (int j = i + 1; j < Num; j++) {
				pos_[j].translation_ = { -100,-100,-100 };
				pos_[j].UpdateMatrix();
			}
			break;
		}
		else if (pos_[i].translation_.y >= wallStart_[3].y) {
			pos_[i].scale_ = { 0.1f,1.0f,1.0f };
			pos_[i].translation_.y = wallStart_[3].y;
			textNum_[i] = 1;
			pos_[i].UpdateMatrix();
			for (int j = i + 1; j < Num; j++) {
				pos_[j].translation_ = { -100,-100,-100 };
				pos_[j].UpdateMatrix();
			}
			break;
		}
	}
	for (int i = 0; i < Num; i++) {
		pos_[i].translation_.z += -1;
		pos_[i].UpdateMatrix();
	}
}

void Guide::Draw(ViewProjection view)
{
	for (int i = 0; i < Num; i++) {
		if (!isLongPush) {
			if (textNum_[i] == 1) {
				model_->Draw(pos_[i], view);
			}
			else {
				model_->Draw(pos_[i], view, texture_);
			}
		}
		else {
			if (textNum_[i] == 1) {
				model_->Draw(pos_[i], view);
			}
			else {
				model_->Draw(pos_[i], view, texture_);
			}
		}
	}
}