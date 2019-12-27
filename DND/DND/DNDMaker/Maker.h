//////////////////////////////////////////////////////////////////////////
//name:		DND Marker
//author:	Lveyou
//date:		19-12-23

//other:
//19-12-23: ���ڹ���ͼ�񡢾��顢���� - Lveyou

//////////////////////////////////////////////////////////////////////////
#pragma once

#include <DND.h>
using namespace DND;

#include <map>
#include <list>
using namespace std;


class Maker;
class SpriteMgr;
class ImageMgr
{
public:
	ImageMgr(Maker* maker) : 
		_maker(maker),
		_curPack(-1)
	{

	}
	void LoadImage(const String& path);

	//������ʼλ�� �� ������
	void Render(Vector2 start, float h);
private:
	Maker* _maker;
	

	struct ImageNode
	{
		Image* _img;
		ButtonSprite3Text1* _btn;
		Sprite* _spr;
	};
	struct ImagePack
	{
		map<String, ImageNode> _mapImages;
		ButtonSprite3Text1* _btn;
		int _curNode;//��ǰѡ���node��Ϊ-1����δѡ��
		ImagePack() : _curNode(-1) {}
	};

	int _curPack;//��ǰѡ���pack��Ϊ-1����δѡ��
	map<String, ImagePack*> _mapPacks;

};


class Maker : public Game
{
public:
	Maker() : _locator(NULL), _sprBg(NULL), _mgrImage(this)
	{
		
	}
	~Maker()
	{
		delete _locator;
		delete _sprBg;
	}

	

	virtual void _update() override;
	virtual void _init() override;
	virtual void _release() override;

	virtual void _on_resize() override;

	void RunOutline();
	void RenderOutline();

	void RunImage();
	void RenderImage();

	void UpdateUI();
	Coor* GetCoorShow()
	{
		return _coorShow;
	}
	Coor* GetCoorRight()
	{
		return _coorRight;
	}
private:

	struct SL
	{
		WCHAR _workPath[MAX_PATH];
		int _menuState;
		int _gridDw;
		int _gridDh;
		DWORD _gridColor;
		bool _gridShow;
		float _gridScale;
		SL()
		{
			_workPath[0] = 0;
			_menuState = -1;
			_gridDw = 32;
			_gridDh = 32;
			_gridColor = 0x66000000;
			_gridShow = true;
			_gridScale = 1.0f;
		}
			
	}_sl;

	

	Locator* _locator;
	Sprite* _sprBg;
	//center��ʱ����
	Coor* _coorRight;
	Coor* _coorShow;

	//����

	//�˵���
	enum MenuBar
	{
		OUTLINE,
		IMAGE,
		SPRITE,
		ANIMATION,
		NUM
	};
	Text* _txtMenu;
	Sprite* _sprBtn01[3];
	Vector2 _offsetMenuBtn;
	ButtonSprite3Text1* _btnMenu[NUM];

	//______________________________OUTLINE_____________________________
	Sprite* _sprBtn02[3];
	Vector2 _offsetOutlineBtn;
	ButtonSprite3Text1* _btnOutline[1];

	Text* _txtWorkPath;
	
	

	//_______________________________IMAGE_______________________________
	Sprite* _sprBtn03[3];
	Vector2 _offsetOnelineBtn;
	Text* _txtOneline;
	ButtonSprite3Text1* _btnTempOneline;
public:
	SpriteMgr* _sprMgr;
	ButtonSprite3Text1* GetTempBtnOneline()
	{
		return _btnTempOneline->Clone();
	}
	ButtonSprite3Text1*  GetTempBtnMenu()
	{
		return _btnMenu[0]->Clone();
	}
private:

	Text* _txtImagePath[2];//�ɹ���·���Զ�����

	//ˢ��
	ButtonSprite3Text1* _btnImageFlush;
	ImageMgr _mgrImage;
public:
	
};