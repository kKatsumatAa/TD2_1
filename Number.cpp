#include "Number.h"
#include "TextureManager.h"

void Number::Initialize()
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			numTexture_ = TextureManager::Load("number.png");
			sprite_[j][i] = Sprite::Create(numTexture_, { 0,0 });
			sprite_[j][i]->SetTextureRect({ (float)48 * i,0 }, { 48,64 });
			sprite_[j][i]->SetSize({ 48,64 });
		}
	}
}

void Number::Update()
{

}

void Number::Draw(Vector2 pos, uint32_t num)
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 5; j++) {
			sprite_[j][i]->SetPosition(pos);
		}
	}
	if (num < 10) {
		sprite_[0][num]->Draw();
	}
	else if (num < 100) {
		sprite_[0][num / 10]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[1][num % 10]->Draw();
	}
	else if (num < 1000) {
		sprite_[0][num / 100]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[1][num / 10 % 10]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[2][num % 10]->Draw();
	}
	else if (num < 10000) {
		sprite_[0][num / 1000]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[1][num / 100 % 10]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[2][num / 10 % 10]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[3][num % 10]->Draw();
	}
	else if (num < 100000) {
		sprite_[0][num / 10000]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[1][num / 1000 % 10]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[2][num / 100 % 10]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[3][num / 10 % 10]->Draw();
		pos.x += 48;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				sprite_[j][i]->SetPosition(pos);
			}
		}
		sprite_[4][num % 10]->Draw();
	}
}
