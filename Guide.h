#pragma once
#include "Vector2.h"
#include "Model.h"

class Guide
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	Vector2 playerStart_;
	Vector2 playerEnd_;
	Vector2 WallStart_[4];
	Vector2 WallEnd_[4];
	Model* model_;
};

