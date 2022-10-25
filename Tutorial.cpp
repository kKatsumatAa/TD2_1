#include "Tutorial.h"
#include "TextureManager.h"

void Tutorial::Initialize()
{
	state = new UITutorial;
	state->SetTutorial(this);

	input = Input::GetInstance();

	//外部から今行ってるチュートリアルが分かるようにした変数
	state2 = RUSH;
	isEnd = false;
	count = 0;

	texhandle[0] = TextureManager::Load("tutorial/test.png");
	texhandle[1] = TextureManager::Load("tutorial/next.png");
	texhandle[2] = TextureManager::Load("tutorial/box.png");

	sprite[0] = Sprite::Create(texhandle[0], { 10,10 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[2] = Sprite::Create(texhandle[2], { 10,100 });
}

void Tutorial::Update()
{
	state->Update(input);
}

void Tutorial::Draw()
{
	state->Draw();

	count += 0.1f;
	//チュートリアル中の表示
	sprite[0]->SetSize({ 100,50 });
	sprite[1]->SetSize({ 150,50 });
	sprite[0]->SetPosition({ 920, (float)320 + sinf(count) * 3.0f });
	sprite[1]->SetPosition({ 920, (float)380 + sinf(count) * 3.0f });

	//sprite[0]->Draw();
	//sprite[1]->Draw();

}


//-----------------------------------------------------------
RushTutorial::RushTutorial()
{
	//RushTutorial用
	texhandle[0] = TextureManager::Load("tutorial/1-1.png");
	texhandle[1] = TextureManager::Load("tutorial/1-2.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
}

void RushTutorial::Update(Input* input)
{
	if (num >= numMax)num = numMax - 1;

	if (input->TriggerKey(DIK_Z) && num == 1)
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
		//next
		if (num == 1) tutorial->sprite[1]->Draw();

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
	if (input->TriggerKey(DIK_Z))
	{
		num++;

		if (num >= numMax)
		{
			tutorial->AddStateNum();
			tutorial->AddState2();
			tutorial->ChangeState(new ItemTutorial);
		}
	}
}

void GravityObjTutorial::Draw()
{
	count += 0.1f;

	if (num < numMax)
	{
		//next
		tutorial->sprite[1]->Draw();

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
		tutorial->ChangeState(new LastTutorial);
	}
}

void UITutorial::Draw()
{
	count += 0.1f;

	if (num < numMax)
	{
		//next
		tutorial->sprite[1]->Draw();
		sprite[num]->SetPosition({ 800 , 450 + sinf(count) * 5.0f });
		sprite[num]->Draw();

		switch (num)
		{
		case 0:
			tutorial->sprite[2]->SetSize({ 500 ,120  });
			tutorial->sprite[2]->SetPosition({ 770 + sinf(count) * 5.0f , 5  });
			tutorial->sprite[2]->Draw();
			break;
		case 1:
			tutorial->sprite[2]->SetSize({ 500 ,140  });
			tutorial->sprite[2]->SetPosition({ 770 + sinf(count) * 5.0f , 100 });
			tutorial->sprite[2]->Draw();
			break;
		case 2:
			tutorial->sprite[2]->SetSize({ 500 ,330  });
			tutorial->sprite[2]->SetPosition({ 770 + sinf(count) * 5.0f , 220 });
			tutorial->sprite[2]->Draw();
			break;
		case 3:
			tutorial->sprite[2]->SetSize({ 500 ,150  });
			tutorial->sprite[2]->SetPosition({ 800 + sinf(count) * 5.0f , 100 });
			tutorial->sprite[2]->Draw();
			break;
		}
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
		//next
		tutorial->sprite[1]->Draw();
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
		//next
		tutorial->sprite[1]->Draw();
		sprite[num]->SetPosition({ 800 , 450 + sinf(count) * 5.0f });
		sprite[num]->Draw();
	}
}
