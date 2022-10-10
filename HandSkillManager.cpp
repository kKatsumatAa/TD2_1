#include "HandSkillManager.h"


void HandSkillManager::Initialize(Model* model, uint32_t* textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
}

void HandSkillManager::SkillGenerate(const Vector3& pos)
{
	//�G�𐶐��A������
	std::unique_ptr<HandSkill> skill = std::make_unique<HandSkill>();
	skill->Initialize(model_, textureHandle_, pos);
	//�G��o�^
	skills.push_back(std::move(skill));
}

void HandSkillManager::Update()
{
	for (std::unique_ptr<HandSkill>& skill : skills)
	{
		skill->Update();
	}

	//�G����
	skills.remove_if([](std::unique_ptr<HandSkill>& skill)
		{
			return (skill->GetIsDead());
		}
	);
}

void HandSkillManager::Draw(const ViewProjection& view)
{
	for (std::unique_ptr<HandSkill>& skill : skills)
	{
		skill->Draw(view);
	}
}
