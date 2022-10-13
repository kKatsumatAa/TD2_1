#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Util.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "Vector3.h"
#include "Gravity.h"

class Wall
{
public:
	void Initialize(Gravity* gravity);
	void Update();
	void Draw(ViewProjection viewProjection);

	Vector3 isCollisionWall(Vector3 pos, const Vector3& velocity,bool* flag = nullptr);
	void Start();

private:
	//�l���̕�
	Model* wall_[5];
	WorldTransform wallTrans_[5];
	Gravity* gravity_;
};

