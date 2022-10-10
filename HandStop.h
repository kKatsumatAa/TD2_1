#pragma once

class HandStop
{
private:
	bool isStop = false;

public:
	void SetIsStop(const bool& isStop) { this->isStop = isStop; }
	bool GetIsStop() { return isStop; }
};

