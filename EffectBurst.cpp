#include "EffectBurst.h"
#include <random>

void Burst::Initialize(Model* model, uint32_t texture, Vector3 pos, float range, float pow) {

	model_ = model;
	texture_ = texture;

	//�����_��
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> x(-range, range);
	std::uniform_real_distribution<float> y(-range, range);
	std::uniform_real_distribution<float> z(-pow, -1.0f);

	std::uniform_real_distribution<float> rx(-0.5f, 0.5f);
	std::uniform_real_distribution<float> ry(-0.5f, 0.5f);
	std::uniform_real_distribution<float> rz(-0.5f, 0.0f);
	//�����_���ŏo�����l������
	move_ = { x(engine), y(engine), z(engine) };
	rotate_ = { rx(engine), ry(engine), rz(engine) };

	//���W�̏�����
	worldTransform_.Initialize();
	//�����̍��W������
	worldTransform_.translation_ = pos;
	//�傫����ύX
	worldTransform_.scale_ = { 0.5f, 0.5f, 0.5f };

	worldTransform_.UpdateMatrix();

}

void Burst::Update() {
	worldTransform_.translation_ += move_;
	move_.z += 0.05f;
	if (worldTransform_.translation_.z > 20) {
		isDead_ = true;
	}
	worldTransform_.rotation_ += rotate_;
	worldTransform_.UpdateMatrix();
}

void Burst::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection/*, texture_*/);
}