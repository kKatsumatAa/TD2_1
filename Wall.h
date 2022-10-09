#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Util.h"
#include "ViewProjection.h"
#include "TextureManager.h"

class Wall
{
public:
	void Initialize();
	void Update();
	void Draw(ViewProjection viewProjection);

private:
	//�l�����͂ޕ�
	Model* wall_[4];
	WorldTransform wallTrans_[4];

};

