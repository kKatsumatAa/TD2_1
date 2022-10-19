#include "Shake.h"
#include <random>

Vector3 Shake::ShakePow() {
	if (timer_ > 0) {
		timer_--;
		//ランダム
		std::random_device seed_gen;
		std::mt19937_64 engine(seed_gen());

		std::uniform_real_distribution<float> x(-1.0f, 1.0f);
		std::uniform_real_distribution<float> y(-1.0f, 1.0f);

		//ランダムで出した値を入れる
		pow_ = { x(engine), y(engine), 0 };
	}
	else {
		pow_ = { 0,0,0 };
	}
	return pow_;
}