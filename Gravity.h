#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"

class Gravity
{
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection view);

public:
	WorldTransform pos_;
	Model* model_;

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
	
	Vector3 ObjMove(WorldTransform world, float gavitySpeed, bool isStop);
public:
	int axis = DOWN;
	bool playerWay = CLOCK;
};

