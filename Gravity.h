#pragma once
#include "Vector3.h"



class Gravity
{
public:
	enum Axis {
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};
	enum Way {
		CLOCK,
		ANTICLOCK,
	};

	Vector3 Move(Vector3 pos, float gravitySpeed,float playerSprrd);
	

public:
	int axis = DOWN;
	bool playerWay = CLOCK;
};

