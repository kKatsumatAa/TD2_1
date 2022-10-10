#include "Setting.h"

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
			sleepTime_ = FPSCount_ / FPS;
			isSetFPS_ = true;
		}
	}
	else {
		FPSCount_++;
	}

	Sleep(sleepTime_);
}
