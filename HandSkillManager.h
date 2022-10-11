#pragma once
#include"HandSkill.h"

class HandSkillManager
{
private:
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t* textureHandle_;
	


public:
	std::list<std::unique_ptr<HandSkill>> skills;


	void SkillGenerate(const Vector3& pos, const float& maxRadius);
	void Initialize(Model* model, uint32_t* textureHandle);
	void Update();
	void Draw(const ViewProjection& view);

	//�e���X�g���擾(const�Q��)
	const std::list<std::unique_ptr<HandSkill>>& GetSkills()
	{
		return skills;
	}
};

