#include "Tutorial.h"
#include "TextureManager.h"

void Tutorial::Initialize()
{
	state = new RushTutorial;
	state->SetTutorial(this);

	input = Input::GetInstance();

	texhandle[0] = TextureManager::Load("tutorial/test.png");
	//texhandle[1] = TextureManager::Load("tutorial/test.png");

	sprite[0] = Sprite::Create(texhandle[0], { 10,10 });
	//sprite[1] = Sprite::Create(texhandle[2], { 10,100 });


}

void Tutorial::Update()
{
	state->Update(input);
}

void Tutorial::Draw()
{
	state->Draw();

	count += 0.1f;
	//�`���[�g���A�����̕\��
	sprite[0]->SetSize({ 100,50 });
	sprite[0]->SetPosition({ 870, (float)380 + sinf(count) * 3.0f });
	//sprite[1]->SetPosition({ 110, (float)160 + sinf(count) * 5.0f });

	sprite[0]->Draw();
	//sprite[1]->Draw();

}


//-----------------------------------------------------------
RushTutorial::RushTutorial()
{
	//RushTutorial�p
	texhandle[0] = TextureManager::Load("tutorial/1-1.png");
	texhandle[1] = TextureManager::Load("tutorial/1-2.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
}

void RushTutorial::Update(Input* input)
{
	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new LongPushTutorial);
	}
}

void RushTutorial::Draw()
{
	count += 0.1f;

	if (num < numMax)
	{
		sprite[num]->SetPosition({ 800 , 450 + sinf(count) * 5.0f });
		sprite[num]->Draw();
	}
}

//-------------------------------------------------------------------------------
LongPushTutorial::LongPushTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/2-1.png");
	texhandle[1] = TextureManager::Load("tutorial/2-2.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
}

void LongPushTutorial::Update(Input* input)
{
	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new GravityObjTutorial);
	}
}

void LongPushTutorial::Draw()
{
	count += 0.1f;

	if (num < numMax)
	{
		sprite[num]->SetPosition({ 800 , 450 + sinf(count) * 5.0f });
		sprite[num]->Draw();
	}
}

//------------------------------------------------------------------
GravityObjTutorial::GravityObjTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/3-1.png");
	texhandle[1] = TextureManager::Load("tutorial/3-2.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
}

void GravityObjTutorial::Update(Input* input)
{
	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new ItemTutorial);
	}
}

void GravityObjTutorial::Draw()
{
	count += 0.1f;

	if (num < numMax)
	{
		sprite[num]->SetPosition({ 800 , 450 + sinf(count) * 5.0f });
		sprite[num]->Draw();
	}
}

//-----------------------------------------------------------------------
ItemTutorial::ItemTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/4-1.png");
	texhandle[1] = TextureManager::Load("tutorial/4-2.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
}

void ItemTutorial::Update(Input* input)
{
	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new UITutorial);
	}
}

void ItemTutorial::Draw()
{
	count += 0.1f;

	if (num < numMax)
	{
		sprite[num]->SetPosition({ 800 , 450 + sinf(count) * 5.0f });
		sprite[num]->Draw();
	}
}

//-------------------------------------------------------------------------
UITutorial::UITutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/5-1.png");
	texhandle[1] = TextureManager::Load("tutorial/5-2.png");
	texhandle[2] = TextureManager::Load("tutorial/5-3.png");
	texhandle[3] = TextureManager::Load("tutorial/5-4.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[2] = Sprite::Create(texhandle[2], { 10,100 });
	sprite[3] = Sprite::Create(texhandle[3], { 10,100 });
}

void UITutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_Z)) AddNum();

	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new SystemTutorial);
	}
}

void UITutorial::Draw()
{
	count += 0.1f;

	if (num < numMax)
	{
		sprite[num]->SetPosition({ 800 , 450 + sinf(count) * 5.0f });
		sprite[num]->Draw();
	}
}

//-----------------------------------------------------------------------------------
SystemTutorial::SystemTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/6-1.png");
	texhandle[1] = TextureManager::Load("tutorial/6-2.png");
	texhandle[2] = TextureManager::Load("tutorial/6-3.png");
	texhandle[3] = TextureManager::Load("tutorial/6-4.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[2] = Sprite::Create(texhandle[2], { 10,100 });
	sprite[3] = Sprite::Create(texhandle[3], { 10,100 });
}

void SystemTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_Z)) AddNum();

	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new LastTutorial);
	}
}

void SystemTutorial::Draw()
{
	count += 0.1f;

	if (num < numMax)
	{
		sprite[num]->SetPosition({ 800 , 450 + sinf(count) * 5.0f });
		sprite[num]->Draw();
	}
}

//----------------------------------------------------------------------------
LastTutorial::LastTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial/7-1.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
}

void LastTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_Z)) AddNum();

	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->SetIsEnd(true);
	}
}

void LastTutorial::Draw()
{
	count += 0.1f;

	if (num < numMax)
	{
		sprite[num]->SetPosition({ 800 , 450 + sinf(count) * 5.0f });
		sprite[num]->Draw();
	}
}