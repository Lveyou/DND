#include "DNDGUIComboBox.h"
#include "DNDGame.h"
#include "DNDInput.h"
#include <list>
#include <xutility>

namespace DND
{

	float COMBOBOX_TEXT_ORDER_DT = 3 * FLT_EPSILON;

	ComboBox* ComboBox::Create(Sprite* top, Sprite* over, Sprite9* under, ButtonSprite3* right, Text* txt, UINT32 dy)
	{
		ComboBox* ret = new ComboBox;
		ret->_coor = Coor::Create();
		ret->_listItem = new std::list<Text*>;

		ret->_sprTop = top;
		ret->_sprTop->GetCoor()->SetParent(ret->_coor);
		ret->_sprTop->SetUI(true);
		

		ret->_sprUnder = under;
		ret->_sprUnder->GetCoor()->SetParent(ret->_coor);
		ret->_sprUnder->SetUI(true);

		ret->_sprOver = over;
		ret->_sprOver->GetCoor()->SetParent(ret->_sprUnder->GetCoor());

		ret->_btnRight = right;
		ret->_btnRight->GetCoor()->SetParent(ret->_coor);

		ret->_txt = txt;
		ret->_txt->GetCoor()->SetParent(ret->_coor);

		ret->_dy = dy == 0 ? over->GetSize().h : dy;
		ret->SetOrder(top->GetOrder());

		return ret;
	}

	void ComboBox::PushBack(const String& str)
	{
		Text* txt = _txt->Clone();
		txt->SetOrder(_sprTop->GetOrder() - COMBOBOX_TEXT_ORDER_DT);
		txt->SetString(str);

		((std::list<Text*>*)_listItem)->push_back(txt);
	}

	void ComboBox::PushFront(const String& str)
	{
		Text* txt = _txt->Clone();
		txt->SetOrder(_sprTop->GetOrder() - COMBOBOX_TEXT_ORDER_DT);
		txt->SetString(str);

		((std::list<Text*>*)_listItem)->push_front(txt);
	}

	void ComboBox::Clear()
	{
		((std::list<Text*>*)_listItem)->clear();
	}

	int ComboBox::Run()
	{
		_sprTop->Render();
		_btnRight->Run();
		
		int ret = -1;
		
		if (_btnRight->IsOpen())
		{
			auto p = (std::list<Text*>*)_listItem;

			//鼠标检测
			Input* input = Game::Get()->input;

			Vector2 pos = _sprUnder->GetCoor()->WorldToThis(input->GetMousePosition());
			float ox = _sprOver->GetCoor()->GetPosition().a;
			int i = int(pos.b / (float)_dy);
			if (pos.b < 0 || pos.a < 0 || pos.a > _sprTop->GetSize().w)
				i = -1;

			if (i < 0 || i >= int(p->size()))
			{
				_sprOver->GetCoor()->SetPosition(Vector2(ox, float(_cur * _dy)));				
			}
			else
			{
				_sprOver->GetCoor()->SetPosition(Vector2(ox, float(i * _dy)));
				if (input->KeyUp(KeyCode::MOUSE_L))
				{
					if (_cur != i)
					{
						_cur = i;
						_txt->SetString(GetCur());
						ret = i;
					}
						
					_cur = i;
					_btnRight->SetOpen(false);
					
				}
			}
					
			_sprOver->Render();

			//定位背景
			_sprUnder->SetPosition(Vector4(0, 0, float(_sprTop->GetSize().w), float(p->size() * _dy)));
			_sprUnder->Render();
			
			//绘制条目
			float x = _txt->GetCoor()->GetPosition().a;
			float y = _sprUnder->GetCoor()->GetPosition().b;

			for (auto& iter : *p)
			{
				iter->GetCoor()->SetPosition(Vector2(x, y));
				iter->Render();
				y += _dy;
			}
		}

		_txt->Render();

		return ret;
	}

	DND::String ComboBox::GetCur()
	{
		auto p = (std::list<Text*>*)_listItem;
		if (p->size() == 0)
			return STRING_EMPTY;

		auto iter = p->begin();
		UINT32 i = 0;

		advance(iter, _cur);

		if (*iter == NULL)
			return STRING_EMPTY;

		return (*iter)->GetString();
	}

	int ComboBox::GetCurNumber()
	{
		return _cur;
	}

	void ComboBox::SetCur(int i)
	{
		_cur = i;
		_txt->SetString(GetCur());
	}

	UINT32 ComboBox::GetSize()
	{
		return ((std::list<Text*>*)_listItem)->size();
	}

	DND::Coor* ComboBox::GetCoor()
	{
		return _coor;
	}

	ComboBox* ComboBox::Clone()
	{
		ComboBox* ret = new ComboBox;
		ret->_coor = _coor->Clone();
		ret->_listItem = new std::list<Text*>;
		*(std::list<Text*>*)(ret->_listItem) = *(std::list<Text*>*)_listItem;

		ret->_sprTop = _sprTop->Clone();
		ret->_sprTop->GetCoor()->SetParent(ret->_coor);

		ret->_sprUnder = _sprUnder->Clone();
		ret->_sprUnder->GetCoor()->SetParent(ret->_coor);

		ret->_sprOver = _sprOver->Clone();
		ret->_sprOver->GetCoor()->SetParent(ret->_sprUnder->GetCoor());

		ret->_btnRight = _btnRight->Clone();
		ret->_btnRight->GetCoor()->SetParent(ret->_coor);

		ret->_txt = _txt->Clone();
		ret->_txt->GetCoor()->SetParent(ret->_coor);

		ret->_dy = _dy;

		return ret;
	}

	void ComboBox::SetOrder(float order)
	{
		_sprTop->SetOrder(order);
		_sprUnder->SetOrder(order - 1 * FLT_EPSILON);
		_sprOver->SetOrder(order - 2 * FLT_EPSILON);
		_btnRight->SetOrder(order - 3 * FLT_EPSILON);
		_txt->SetOrder(order - COMBOBOX_TEXT_ORDER_DT);

		auto* p = (std::list<Text*>*)_listItem;
		for (auto& iter : *p)
		{
			iter->SetOrder(order - COMBOBOX_TEXT_ORDER_DT);
		}
	}

	ComboBox::ComboBox()
	{
		_sprTop = NULL;
		_btnRight = NULL;
		_sprUnder = NULL;
		_sprOver = NULL;

		_coor = NULL;
		_cur = -1;
	}

	

}
