#pragma once
#include "Vector3.h"

class Shake {
public:
	Vector3 ShakePow();
	void SetTimer(int time) {
		timer_ = time;
	};
private:
	int timer_ = 0;
	Vector3 pow_ = Vector3(0,0,0);
};