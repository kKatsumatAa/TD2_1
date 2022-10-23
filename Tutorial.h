#pragma once
#include"Input.h"
#include"Sprite.h"

static enum TUTORIAL
{
	RUSH,
	LONG_PUSH,
	ITEM,
	UI,
	SYSTEM,
	LAST
};

class Tutorial;

class TutorialState
{
protected:
	Tutorial* tutorial;
	float count = 0;

public:
	void SetTutorial(Tutorial* tutorial) { this->tutorial = tutorial; }
	virtual void Update(Input* input) = 0;
	virtual void Draw() = 0;

	virtual void AddNum() = 0;
	virtual int GetNum() = 0;
	virtual int GetMaxNum() = 0;
};


class Tutorial
{
private:
	//シンプルなステート（処理用
	TutorialState* state = nullptr;
	//外部から今行ってるチュートリアルが分かるようにした変数
	int state2 = RUSH;
	bool isEnd = false;
	float count = 0;
	Input* input = nullptr;

	unsigned int texhandle[10];
	Sprite* sprite[5];
public:
	

	void Initialize();
	void Update();
	void Draw();

	int GetState() { return state2; }
	void AddState2() { state2++; }
	void AddState() { state->AddNum(); }

	int GetStateNum() { return state->GetNum(); }
	int GetStateNumMax() { return state->GetMaxNum(); }

	bool GetIsEnd() { return isEnd; }

	void SetIsEnd(bool isEnd) { this->isEnd = isEnd; }

	void ChangeState(TutorialState* state)
	{
		delete this->state;
		this->state = state;
		state->SetTutorial(this);
	}
};


//----------------------------------------------------------
class RushTutorial :public TutorialState
{
private:
	int num = 0;
	const int numMax = 2;
	int texhandle[5];
	Sprite* sprite[5];

public:
	RushTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update(Input* input) override;
	void Draw() override;
};

class LongPushTutorial : public TutorialState
{
private:
	int num = 0;
	const int numMax = 2;
	int texhandle[5];
	Sprite* sprite[5];

public:
	LongPushTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update(Input* input) override;
	void Draw() override;
};
