#include <DND.h>
using namespace DND;

const UINT32 GAME_SPR_NUM = 5;
class T01 : public Game
{
public:
	Sprite* _arrSpr[GAME_SPR_NUM];
	Text* _txtLT;
	ButtonTextColor* _btn;

	T01()
	{
		for (auto& iter : _arrSpr)
			iter = NULL;
		_txtLT = NULL;
	}

	virtual void _update() override
	{//帧函数
		//记录位置
		Vector2 pos = _arrSpr[1]->GetCoor()->GetPosition();
		//计算位移，这里是1秒钟移动100像素
		//time->GetRealDelta()返回的是一帧的时间
		float dis_dt = 100.0f * time->GetRealDelta();

		//判断按键是否按下
		if (input->KeyState(KeyCode::W))
			pos.b -= dis_dt;
		if (input->KeyState(KeyCode::S))
			pos.b += dis_dt;
		if (input->KeyState(KeyCode::A))
			pos.a -= dis_dt;
		if (input->KeyState(KeyCode::D))
			pos.a += dis_dt;

		//旋转
		float r = _arrSpr[1]->GetCoor()->GetRotate();
		//每秒旋转 四分之一PI
		float r_dt = Math::GetPI<1, 4>() * time->GetRealDelta();
		if (input->KeyState(KeyCode::Q))
			r -= r_dt;
		if (input->KeyState(KeyCode::E))
			r += r_dt;

		//缩放
		float scale = _arrSpr[1]->GetCoor()->GetScale().a;
		//每秒缩放0.5
		float scale_dt = 0.5f * time->GetRealDelta();
		if (input->KeyState(KeyCode::Z))
			scale -= scale_dt;
		if (input->KeyState(KeyCode::X))
			scale += scale_dt;


		//设置属性
		_arrSpr[1]->GetCoor()->SetPosition(pos);
		_arrSpr[1]->GetCoor()->SetRotate(r);
		_arrSpr[1]->GetCoor()->SetScale({scale, scale });

		_btn->Run();
		//点击并释放
		if (_btn->IsRelease())
		{
			FileNameType types[] = {
				{ L"波形文件", L"*.wav"},
				{ L"所有文件", L"*.*" }
			};
			//让用户选择文件
			String file_name = sys->GetChooseFile(false, types, _countof(types));
			if (file_name.GetLength())
			{//成功选择了
				debug_msg(file_name);
				static int i = 0;
				String name = String(L"snd_") + i++;
				sound->Load(name, file_name);
				snd_play(name);
			}
		}
		
		for (UINT32 i = 0; i != GAME_SPR_NUM; ++i)
		{
			if(_arrSpr[i])
				_arrSpr[i]->Render();
		}
		_txtLT->Render();
	}
	virtual void _init() override
	{//初始化
		sound->SetOpen(true);

		sys->LoadFontFile(L"01", L"Data\\Font\\simkai.ttf");

		_txtLT = canvas->CreateText(L"01", 24);
		_txtLT->SetString(L"按WASD移动，QE旋转，ZX缩放");

		_arrSpr[0] = canvas->CreateSprite(0, Quad({}, {64, 64}, true), Color::GREEN);
		_arrSpr[0]->GetCoor()->SetPosition({ 400, 300 });

		Image* img = Image::Create(L"Data\\Image\\cat.png");
		_arrSpr[1] = canvas->CreateSprite(img);
		_arrSpr[1]->GetCoor()->SetPosition({ 400, 300 });
		delete img;

		_arrSpr[0]->GetCoor()->SetParent(_arrSpr[1]->GetCoor());
		_arrSpr[0]->GetCoor()->SetPosition({ 100, 100 });

		Text* btn_text = canvas->CreateText(L"01", 18);
		btn_text->SetString(L"选择音效");
		_btn = ButtonTextColor::Create(btn_text, Color::GREEN, Color::YELLOW, Color::BLUE);
		_btn->GetText()->GetCoor()->SetPosition({ 400, 500 });

		sys->SetWindowTitle(L"教程01");//设置窗口标题
		sys->SetWindowStyle(WS_OVERLAPPEDWINDOW);//设置窗口样式
		sys->SetWindowSize({800, 600});//设置窗口大小为800*600
		sys->SetWindowCenter();//居中窗口
		sys->SetWindowShow(true);//显示窗口

		time->SetFPS(120);//设为0以不限制帧数（默认为60）
	}
	virtual void _release() override
	{//结束
		for (auto& iter : _arrSpr)
			DELETE_SPRITE(iter);

		delete _txtLT;
	}
};

T01 g_t01;

DNDMain()
{
	DebuggerConsole debug;
	Debug::SetDebugger(&debug);

	g_t01.Init();
	g_t01.EnterLoop();
	g_t01.Release();

	//MessageBox(NULL, L"Hello World!", L"标题栏", MB_OK);
}