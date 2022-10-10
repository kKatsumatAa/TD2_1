#pragma once
#include <time.h>
#include "DirectXCommon.h"
#include "Vector2.h"
#include "DebugText.h"

class Setting
{
public:
	void Initialize();

	void FPS(float FPS);
	void DrawFPS(Vector2 pos = {1180,0});
private:
	time_t old_ = time(NULL);
	time_t now_ = time(NULL);
	float FPSCount_ = 0.0f;
	float drawCount_ = 0.0f;
	float showFPSCount_ = 0.0f;
	float sleepTime_ = 0.0f;
	bool isSetFPS_ = false;
	bool isStart_ = false;

	DebugText* debugText_ = nullptr;
};

