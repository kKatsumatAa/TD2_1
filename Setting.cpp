#include "Setting.h"

void Setting::Initialize() {
	debugText_ = DebugText::GetInstance();
}

void Setting::FPS(float FPS)
{
	now_ = time(NULL);
	if (old_ != now_ && isSetFPS_ == false) {
		if (isStart_ == false) {
			isStart_ = true;
			FPSCount_ = 0;
			old_ = now_;
		}
		else {
			old_ = time(NULL);
			sleepTime_ = (FPSCount_ / FPS) * 2;
			isSetFPS_ = true;
		}
	}
	else {
		FPSCount_++;
	}

	Sleep(sleepTime_);
}

void Setting::DrawFPS(Vector2 pos) {
	now_ = time(NULL);
	drawCount_++;
	if (old_ != now_) {
		showFPSCount_ = drawCount_;
		drawCount_ = 0;
		old_ = now_;
	}

	debugText_->SetPos(pos.x, pos.y);
	debugText_->Printf("FPS = %03.0f", showFPSCount_);
}