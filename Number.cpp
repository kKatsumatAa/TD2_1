#include "Number.h"
#include "TextureManager.h"

void Number::Initialize(Sprite* sprite)
{
	sprite_ = sprite;
	numTexture_ = TextureManager::Load("number.png");
}

void Number::Update()
{

}

void Number::Draw()
{

}
