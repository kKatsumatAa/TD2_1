#pragma once
#include"HandSkill.h"

class HandSkillManager
{
private:
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t* textureHandle_;
	


public:
	std::list<std::unique_ptr<HandSkill>> skills;


	void SkillGenerate(const Vector3& pos, const float& maxRadius);
	void Initialize(Model* model, uint32_t* textureHandle);
	void Update();
	void Draw(const ViewProjection& view);

	//弾リストを取得(const参照)
	const std::list<std::unique_ptr<HandSkill>>& GetSkills()
	{
		return skills;
	}
};

