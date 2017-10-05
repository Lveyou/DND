#include "test.h"
#include "SceneInput.h"



void SceneInput::Init( Test* test )
{
	_test = test;
	_test->input->OpenGamePad();

	Image* img = Image::Create(L"Data\\Image\\gamepad.png");
	_sprGamePad[0] = _test->canvas->CreateSprite(img);
	_sprGamePad[1] = _sprGamePad[0]->Clone();
	_sprGamePad[2] = _sprGamePad[0]->Clone();
	_sprGamePad[3] = _sprGamePad[0]->Clone();

	_sprGamePad[0]->GetCoor()->SetPosition(Vector2(400, 100));
	_sprGamePad[1]->GetCoor()->SetPosition(Vector2(500, 100));
	_sprGamePad[2]->GetCoor()->SetPosition(Vector2(600, 100));
	_sprGamePad[3]->GetCoor()->SetPosition(Vector2(700, 100));

	_txtTitle = _test->canvas->CreateText(GAME_FONT_NAME_TEXT, GAME_FONT_SIZE_TEXT);
	_txtTitle->SetMaxW(500);
	_txtTitle->SetString(L"RS控制鼠标，LS设置图标位置，A、B旋转图标，X、Y缩放图标，LT、RT震动马达。");
	_txtTitle->GetCoor()->SetPosition(Vector2(250, 20));
}

void SceneInput::Run()
{
	UINT32 i = 0;
	for (auto& iter : _sprGamePad)
	{
		_sprGamePad[i++]->SetColor(0x66ffffff);
		iter->Render();
	}

	if (GamePad* game_pad_01 = _test->input->GetGamePad(0))
	{
		//
		if (game_pad_01->KeyState(PadCode::UP))
		{
			//Point window_pos = _test->sys->get
		}
		//RS控制鼠标
		Vector2 rs = game_pad_01->GetRS();
		Vector2 pos = _test->input->GetMousePosition();
		pos = pos + rs * (1000 * _test->time->GetRealDelta());
		_test->input->SetMousePosition(Vector2ToPoint(pos));

		//LT,LB振动
		game_pad_01->SetVibration(Vector2(game_pad_01->ForceLT(), game_pad_01->ForceRT()));

		//LS设置图标位置
		Vector2 ls = game_pad_01->GetLS();
		Vector2 pos2 = _sprGamePad[0]->GetCoor()->GetPosition();
		pos2 = pos2 + ls * (1000 * _test->time->GetRealDelta());
		_sprGamePad[0]->GetCoor()->SetPosition(pos2);

		//XY大小
		if (game_pad_01->KeyState(PadCode::X))
		{
			float dt = _test->time->GetRealDelta() * 5.0f;
			_sprGamePad[0]->GetCoor()->SetScale(_sprGamePad[0]->GetCoor()->GetScale() + Vector2(dt, dt));
		}
		if (game_pad_01->KeyState(PadCode::Y))
		{
			float dt = -_test->time->GetRealDelta() * 5.0f;
			_sprGamePad[0]->GetCoor()->SetScale(_sprGamePad[0]->GetCoor()->GetScale() + Vector2(dt, dt));
		}

		//AB旋转
		if (game_pad_01->KeyState(PadCode::A))
		{
			float dt = -_test->time->GetRealDelta() * 3.14f;
			_sprGamePad[0]->GetCoor()->SetRotate(_sprGamePad[0]->GetCoor()->GetRotate() + dt);
		}
		if (game_pad_01->KeyState(PadCode::B))
		{
			float dt = _test->time->GetRealDelta() * 3.14f;
			_sprGamePad[0]->GetCoor()->SetRotate(_sprGamePad[0]->GetCoor()->GetRotate() + dt);
		}

		_sprGamePad[0]->SetColor(Color::WHITE);
	}	


	_txtTitle->Render();
}
