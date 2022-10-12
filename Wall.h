#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Util.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Vector3.h"

class Wall
{
public:
	void Initialize();
	void Update();
	void Draw(ViewProjection viewProjection);

	Vector3 isCollisionWall(Vector3 pos, const Vector3& velocity,bool* flag = nullptr);
	void Start();

private:
	//Žl•û‚Ì•Ç
	Model* wall_[4];
	WorldTransform wallTrans_[4];

};

