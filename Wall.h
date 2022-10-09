#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Util.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Vector2.h"

class Wall
{
public:
	void Initialize();
	void Update();
	void Draw(ViewProjection viewProjection);

	bool isCollisionWall(Vector2 pos);

private:
	//Žl•û‚Ì•Ç
	Model* wall_[4];
	WorldTransform wallTrans_[4];

};

