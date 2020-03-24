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
	{//֡����
		//��¼λ��
		Vector2 pos = _arrSpr[1]->GetCoor()->GetPosition();
		//����λ�ƣ�������1�����ƶ�100����
		//time->GetRealDelta()���ص���һ֡��ʱ��
		float dis_dt = 100.0f * time->GetRealDelta();

		//�жϰ����Ƿ���
		if (input->KeyState(KeyCode::W))
			pos.b -= dis_dt;
		if (input->KeyState(KeyCode::S))
			pos.b += dis_dt;
		if (input->KeyState(KeyCode::A))
			pos.a -= dis_dt;
		if (input->KeyState(KeyCode::D))
			pos.a += dis_dt;

		//��ת
		float r = _arrSpr[1]->GetCoor()->GetRotate();
		//ÿ����ת �ķ�֮һPI
		float r_dt = Math::GetPI<1, 4>() * time->GetRealDelta();
		if (input->KeyState(KeyCode::Q))
			r -= r_dt;
		if (input->KeyState(KeyCode::E))
			r += r_dt;

		//����
		float scale = _arrSpr[1]->GetCoor()->GetScale().a;
		//ÿ������0.5
		float scale_dt = 0.5f * time->GetRealDelta();
		if (input->KeyState(KeyCode::Z))
			scale -= scale_dt;
		if (input->KeyState(KeyCode::X))
			scale += scale_dt;


		//��������
		_arrSpr[1]->GetCoor()->SetPosition(pos);
		_arrSpr[1]->GetCoor()->SetRotate(r);
		_arrSpr[1]->GetCoor()->SetScale({scale, scale });

		_btn->Run();
		//������ͷ�
		if (_btn->IsRelease())
		{
			FileNameType types[] = {
				{ L"�����ļ�", L"*.wav"},
				{ L"�����ļ�", L"*.*" }
			};
			//���û�ѡ���ļ�
			String file_name = sys->GetChooseFile(false, types, _countof(types));
			if (file_name.GetLength())
			{//�ɹ�ѡ����
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
	{//��ʼ��
		sound->SetOpen(true);

		sys->LoadFontFile(L"01", L"Data\\Font\\simkai.ttf");

		_txtLT = canvas->CreateText(L"01", 24);
		_txtLT->SetString(L"��WASD�ƶ���QE��ת��ZX����");

		_arrSpr[0] = canvas->CreateSprite(0, Quad({}, {64, 64}, true), Color::GREEN);
		_arrSpr[0]->GetCoor()->SetPosition({ 400, 300 });

		Image* img = Image::Create(L"Data\\Image\\cat.png");
		_arrSpr[1] = canvas->CreateSprite(img);
		_arrSpr[1]->GetCoor()->SetPosition({ 400, 300 });
		delete img;

		_arrSpr[0]->GetCoor()->SetParent(_arrSpr[1]->GetCoor());
		_arrSpr[0]->GetCoor()->SetPosition({ 100, 100 });

		Text* btn_text = canvas->CreateText(L"01", 18);
		btn_text->SetString(L"ѡ����Ч");
		_btn = ButtonTextColor::Create(btn_text, Color::GREEN, Color::YELLOW, Color::BLUE);
		_btn->GetText()->GetCoor()->SetPosition({ 400, 500 });

		sys->SetWindowTitle(L"�̳�01");//���ô��ڱ���
		sys->SetWindowStyle(WS_OVERLAPPEDWINDOW);//���ô�����ʽ
		sys->SetWindowSize({800, 600});//���ô��ڴ�СΪ800*600
		sys->SetWindowCenter();//���д���
		sys->SetWindowShow(true);//��ʾ����

		time->SetFPS(120);//��Ϊ0�Բ�����֡����Ĭ��Ϊ60��
	}
	virtual void _release() override
	{//����
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

	//MessageBox(NULL, L"Hello World!", L"������", MB_OK);
}