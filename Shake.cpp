#include "Shake.h"
#include <random>

Vector3 Shake::ShakePow() {
	if (timer_ > 0) {
		timer_--;
		//�����_��
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());

		std::uniform_real_distribution<float> x(-0.75f, 0.75f);
		std::uniform_real_distribution<float> y(-0.75f, 0.75f);

		//�����_���ŏo�����l������
		pow_ = { x(engine), y(engine), 0 };
	}
	else {
		pow_ = { 0,0,0 };
	}
	return pow_;
}