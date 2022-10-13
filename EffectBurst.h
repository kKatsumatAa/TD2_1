#pragma once
#include "WorldTransform.h"
#include "Vector3.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Util.h"

class Burst {
public:
	void Initialize(Model* model,uint32_t texture, Vector3 pos,float range,float pow);
	void Update();
	void Draw(ViewProjection viewProjection);

	bool IsDead() const {
		return isDead_;
	}
private:
	//���W
	WorldTransform worldTransform_;
	//�ړ�
	Vector3 move_;
	//��]
	Vector3 rotate_;
	//���f��
	Model* model_;
	//�e�N�X�`��
	uint64_t texture_;
	//���Ńt���O
	bool isDead_;
};