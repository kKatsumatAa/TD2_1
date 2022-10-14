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

	Vector3 Move(Vector3 pos, float speed);
	void Change(int axis);

public:
	int axis = DOWN;

};

