#pragma once
#include <time.h>
#include "DirectXCommon.h"
class Setting
{
public:
	void FPS(float FPS);

private:
	time_t old_ = time(NULL);
	time_t now_ = time(NULL);
	float FPSCount_ = 0.0f;
	float sleepTime_ = 0.0f;
	bool isSetFPS_ = false;
	bool isStart_ = false;
};

