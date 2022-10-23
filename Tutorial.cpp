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
	//チュートリアル中の表示
	sprite[0]->SetPosition({ 50, (float)50 + sinf(count) * 5.0f });
	//sprite[1]->SetPosition({ 110, (float)160 + sinf(count) * 5.0f });

	sprite[0]->Draw();
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
	//クリックしてせつめい画像を進める
	if (input->TriggerKey(DIK_SPACE)) num++;

	if (num >= numMax)
	{
		tutorial->AddState();
		tutorial->AddState2();
		tutorial->ChangeState(new LongPushTutorial);
	}
}

void RushTutorial::Draw()
{
	count += 0.1f;

	if (num == 0)
	{
		sprite[0]->SetPosition({ 500 + sinf(count) * 5.0f, 250 + sinf(count) * 5.0f });
		sprite[0]->Draw();
	}
	if (num == 1)
	{
		sprite[1]->SetPosition({ 500 + sinf(count) * 5.0f, 230 + sinf(count) * 5.0f });
		sprite[1]->Draw();
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
	//クリックしてせつめい画像を進める
	if (input->TriggerKey(DIK_SPACE)) num++;

	if (num >= numMax)
	{
		tutorial->AddState();
		tutorial->AddState2();
		//tutorial->ChangeState(new LongPushTutorial);
	}
}

void LongPushTutorial::Draw()
{
	count += 0.1f;

	if (num == 0)
	{
		sprite[0]->SetPosition({ 500 + sinf(count) * 5.0f, 250 + sinf(count) * 5.0f });
		sprite[0]->Draw();
	}
	if (num == 1)
	{
		sprite[1]->SetPosition({ 500 + sinf(count) * 5.0f, 230 + sinf(count) * 5.0f });
		sprite[1]->Draw();
	}
}
