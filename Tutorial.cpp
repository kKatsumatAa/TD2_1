#include "Tutorial.h"
#include "TextureManager.h"

void Tutorial::Initialize(Audio* audio, uint32_t* soundDataHandle, uint32_t* voiceHandle)
{
	state = new RushTutorial;
	state->SetTutorial(this);

	this->audio = audio;
	this->soundDataHandle = soundDataHandle;
	this->voiceHandle = voiceHandle;

	input = Input::GetInstance();

	//外部から今行ってるチュートリアルが分かるようにした変数
	state2 = RUSH;
	isEnd = false;
	count = 0;
	skipTimer = 0;

	texhandle[0] = TextureManager::Load("skip.png");
	texhandle[1] = TextureManager::Load("box.png");
	texhandle[2] = TextureManager::Load("tutorial/box.png");

	sprite[0] = Sprite::Create(texhandle[0], { 10,10 });
	sprite[1] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[2] = Sprite::Create(texhandle[2], { 10,100 });
}

void Tutorial::Update()
{
	state->Update(input);

	if (input->PushKey(DIK_X))
		skipTimer++;
	else
		skipTimer = 0;

	if (skipTimer >= skipMaxTime) isEnd = true;

	if (input->TriggerKey(DIK_Z))
	{
		voiceHandle[7] = audio->PlayWave(soundDataHandle[7]);
	}
}

void Tutorial::Draw()
{
	state->Draw();

	count += 0.1f;
	//チュートリアル中の表示
	sprite[0]->SetSize({ 192,63 });//384,125
	sprite[0]->SetPosition({ 30, (float)30 + sinf(count) * 3.0f });

	sprite[1]->SetSize({ 192.0f * ((float)skipTimer / (float)skipMaxTime),15 });
	sprite[1]->SetPosition({ 30, (float)30 + sinf(count) * 3.0f });

	sprite[0]->Draw();
	sprite[1]->Draw();

}


//-----------------------------------------------------------
RushTutorial::RushTutorial()
{
	//RushTutorial用
	texhandle[0] = TextureManager::Load("tutorial_1.png");
	texhandle[1] = TextureManager::Load("tutorial_2.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[0]->SetTextureRect({ 0,0 }, { width,height });
	sprite[0]->SetSize({ 315, 270 });
	sprite[1] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1]->SetTextureRect({ width,0 }, { width,height });
	sprite[1]->SetSize({ 315, 270 });
	//
	sprite[2] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[2]->SetTextureRect({ 0,0 }, { width,height });
	sprite[2]->SetSize({ 315, 270 });
	sprite[3] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[3]->SetTextureRect({ width,0 }, { width,height });
	sprite[3]->SetSize({ 315, 270 });
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

	for (int i = 0; i < 4; i++)
	{
		sprite[i]->SetPosition({ 650 , 450 + sinf(count) * 5.0f });
	}

	if (num == 0)
	{
		if ((int)count % 10 >= 5) sprite[0]->Draw();
		else                       sprite[1]->Draw();
	}
	if (num == 1)
	{
		if ((int)count % 10 >= 5) sprite[2]->Draw();
		else                       sprite[3]->Draw();
	}
}

//-------------------------------------------------------------------------------
LongPushTutorial::LongPushTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial_3.png");
	texhandle[1] = TextureManager::Load("tutorial_4.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[0]->SetTextureRect({ 0,0 }, { width,height });
	sprite[0]->SetSize({ 315, 270 });
	sprite[1] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1]->SetTextureRect({ width,0 }, { width,height });
	sprite[1]->SetSize({ 315, 270 });
	//
	sprite[2] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[2]->SetTextureRect({ 0,0 }, { width,height });
	sprite[2]->SetSize({ 315, 270 });
	sprite[3] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[3]->SetTextureRect({ width,0 }, { width,height });
	sprite[3]->SetSize({ 315, 270 });
}

void LongPushTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_Z) && num == 1)
	{
		num++;
	}

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

	for (int i = 0; i < 4; i++)
	{
		sprite[i]->SetPosition({ 650 , 450 + sinf(count) * 5.0f });
	}

	if (num == 0)
	{
		if ((int)count % 10 >= 5) sprite[0]->Draw();
		else                       sprite[1]->Draw();
	}
	if (num == 1)
	{
		if ((int)count % 10 >= 5) sprite[2]->Draw();
		else                       sprite[3]->Draw();
	}
}

//------------------------------------------------------------------
GravityObjTutorial::GravityObjTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial_5.png");
	texhandle[1] = TextureManager::Load("tutorial_6.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[0]->SetTextureRect({ 0,0 }, { width,height });
	sprite[0]->SetSize({ 315, 270 });
	sprite[1] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1]->SetTextureRect({ width,0 }, { width,height });
	sprite[1]->SetSize({ 315, 270 });
	//
	sprite[2] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[2]->SetTextureRect({ 0,0 }, { width,height });
	sprite[2]->SetSize({ 315, 270 });
	sprite[3] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[3]->SetTextureRect({ width,0 }, { width,height });
	sprite[3]->SetSize({ 315, 270 });
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

	for (int i = 0; i < 4; i++)
	{
		sprite[i]->SetPosition({ 650 , 450 + sinf(count) * 5.0f });
	}

	if (num == 0)
	{
		if ((int)count % 10 >= 5) sprite[0]->Draw();
		else                       sprite[1]->Draw();
	}
	if (num == 1)
	{
		if ((int)count % 10 >= 5) sprite[2]->Draw();
		else                       sprite[3]->Draw();
	}
}

//-----------------------------------------------------------------------
ItemTutorial::ItemTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial_7.png");
	texhandle[1] = TextureManager::Load("tutorial_8.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[0]->SetTextureRect({ 0,0 }, { width,height });
	sprite[0]->SetSize({ 315, 270 });
	sprite[1] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1]->SetTextureRect({ width,0 }, { width,height });
	sprite[1]->SetSize({ 315, 270 });
	//
	sprite[2] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[2]->SetTextureRect({ 0,0 }, { width,height });
	sprite[2]->SetSize({ 315, 270 });
}

void ItemTutorial::Update(Input* input)
{
	if (input->TriggerKey(DIK_Z) && num == 0)
	{
		num++;
	}

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

	for (int i = 0; i < 3; i++)
	{
		sprite[i]->SetPosition({ 650 , 450 + sinf(count) * 5.0f });
	}

	if (num == 0)
	{
		if ((int)count % 10 >= 5) sprite[0]->Draw();
		else                       sprite[1]->Draw();
	}
	if (num == 1)
	{
		sprite[2]->Draw();

	}
}

//-------------------------------------------------------------------------
UITutorial::UITutorial()
{
	texhandle[0] = TextureManager::Load("tutorial_10.png");
	texhandle[1] = TextureManager::Load("tutorial_11.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[0]->SetTextureRect({ 0,0 }, { width,height });
	sprite[0]->SetSize({ 315, 270 });
	sprite[1] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1]->SetTextureRect({ width,0 }, { width,height });
	sprite[1]->SetSize({ 315, 270 });
	//
	sprite[2] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[2]->SetTextureRect({ 0,0 }, { width,height });
	sprite[2]->SetSize({ 315, 270 });
	sprite[3] = Sprite::Create(texhandle[1], { 10,100 });
	sprite[3]->SetTextureRect({ width,0 }, { width,height });
	sprite[3]->SetSize({ 315, 270 });
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

	for (int i = 0; i < 4; i++)
	{
		sprite[i]->SetPosition({ 650 , 450 + sinf(count) * 5.0f });
	}



	if (num < numMax)
	{
		switch (num)
		{
		case 0:
			tutorial->sprite[2]->SetSize({ 500 ,140 });
			tutorial->sprite[2]->SetPosition({ 770 + sinf(count) * 5.0f , 100 });
			tutorial->sprite[2]->Draw();
			break;
		case 1:
			tutorial->sprite[2]->SetSize({ 500 ,350 });
			tutorial->sprite[2]->SetPosition({ 770 + sinf(count) * 5.0f , 230 });
			tutorial->sprite[2]->Draw();
			break;
		}


		if (num == 0)
		{
			if ((int)count % 10 >= 5) sprite[0]->Draw();
			else                      sprite[1]->Draw();
		}
		if (num == 1)
		{
			if ((int)count % 10 >= 5) sprite[2]->Draw();
			else                      sprite[3]->Draw();
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
		sprite[num]->SetPosition({ 800 , 400 + sinf(count) * 5.0f });
		sprite[num]->Draw();
	}
}

//----------------------------------------------------------------------------
LastTutorial::LastTutorial()
{
	texhandle[0] = TextureManager::Load("tutorial_9.png");
	sprite[0] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[0]->SetTextureRect({ 0,0 }, { width,height });
	sprite[0]->SetSize({ 315, 270 });
	sprite[1] = Sprite::Create(texhandle[0], { 10,100 });
	sprite[1]->SetTextureRect({ width,0 }, { width,height });
	sprite[1]->SetSize({ 315, 270 });
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

	for (int i = 0; i < 2; i++)
	{
		sprite[i]->SetPosition({ 650 , 450 + sinf(count) * 5.0f });
	}

	if (num == 0)
	{
		if ((int)count % 10 >= 5) sprite[0]->Draw();
		else                       sprite[1]->Draw();
	}
}
