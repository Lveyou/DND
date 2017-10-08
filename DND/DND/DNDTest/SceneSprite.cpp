#include "SceneSprite.h"

void SceneSprite::Init(Test* test)
{
	int x = 300, y = 200;
	_test = test;

	_txtTitle = _test->canvas->CreateText(GAME_FONT_NAME_TEXT, GAME_FONT_SIZE_TEXT);
	_txtTitle->SetMaxW(500);
	_txtTitle->SetString(L"精灵操作展示");
	_txtTitle->GetCoor()->SetPosition(Vector2(250, 20));

	Image* img = Image::Create(L"Data\\Image\\sprite.png");
	_test->canvas->RegisterImageAll(GAME_SCENE_SPRITE_IMAGE_REG_ID + 1, img);
	img->GetSize();

	_spr = _test->canvas->CreateSprite(GAME_SCENE_SPRITE_IMAGE_REG_ID + 1, Quad(Vector2(), img->GetSize(), false));
	_spr->GetCoor()->SetPosition(Vector2(x, y));

	Color array_color[] = {Color::GREEN,
		Color::BLUE, 
		Color::YELLOW, 
		Color::RED };
	//_sprScale = _test->canvas->CreateSprite(GAME_SCENE_SPRITE_IMAGE_REG_ID + 1, Quad(Vector2(), img->GetSize() *2 , false));
	_sprScale = _test->canvas->CreateSprite(GAME_SCENE_SPRITE_IMAGE_REG_ID + 1, Quad(Vector2(0,0),Vector2(300,0),
		Vector2(400,200), Vector2(0,300)));
	_sprScale->SetColor(array_color);
	_sprScale->GetCoor()->SetScale(Vector2(1.5f, 1.5f));
	_sprScale->GetCoor()->SetPosition(Vector2(x - 200, y));


	_sprRotate = _test->canvas->CreateSprite(GAME_SCENE_SPRITE_IMAGE_REG_ID + 1, Quad(Vector2(), img->GetSize(), false));
	_sprRotate->GetCoor()->SetPosition(Vector2(x + 400, y));
	_sprRotate->GetCoor()->SetRotate(3.14159 / 3);

	_sprSetColor = _test->canvas->CreateSprite(GAME_SCENE_SPRITE_IMAGE_REG_ID + 1, Quad(Vector2(), img->GetSize(), false));
	_sprSetColor->GetCoor()->SetPosition(Vector2(x, y + 200));
	_sprSetColor->SetColor(Color::GREEN);


	delete img;
}

void SceneSprite::Run()
{
	_txtTitle->Render();
	_spr->Render();
	_sprScale->Render();
	_sprRotate->Render();
	_sprSetColor->Render();

}
