#include "DNDText_imp.h"

#include "DNDCanvas_imp.h"
#include "DNDDebug.h"

#include "DNDInput.h"
#include "DNDMath.h"
#include "DNDGame.h"
#include "DNDSystem.h"
#include <assert.h>

namespace DND
{
	void Text_imp::Render()
	{
		Sprite* spr = NULL;
		list<Sprite*>::iterator itor;
		for (itor = m_sprites.begin(); itor != m_sprites.end(); ++itor)
		{
			spr = *itor;
			spr->Render();
		}

		if (_showOutLine)
		{
			for (auto& iter : _txtOutline)
			{
				iter->Render();
			}
		}
	}

	void Text_imp::RenderFrame()
	{
		Sprite* spr = NULL;
		list<Sprite*>::iterator itor;
		for (itor = m_sprites.begin(); itor != m_sprites.end(); ++itor)
		{
			spr = *itor;
			spr->RenderFrame();
		}
	}

	void Text_imp::SetString(const String& str)
	{
		if (m_string == str)
			return;
		RegisterString(str);
		SetStringFast(str);
	}

	void Text_imp::SetStringFast(const String& str)
	{
		
		//这个函数排好队形，对齐后面只是统一偏移坐标
		/*if (!str.Get_Length())
		{
		return;
		}*/
		if (m_string == str)
			return;

		m_h = 0;
		m_string = str;
		const wchar_t* buffer = str.GetWcs();
		Sprite* spr = NULL;
		Coor* coor = NULL;
		//输出位置
		unsigned x = 0;
		unsigned x_max = 0;
		unsigned y = 0;
		//标记上次所有精灵 为 已废弃
		for (auto& iter : m_sprites)
		{
			delete iter;
		}
		m_sprites.clear();
		INT32 dw, dh;
		unsigned space_w = (m_font_size >> 2);
		dh = static_cast<int>(m_font_size * m_pitch_row);
		for (int i = 0; buffer[i]; ++i)
		{
			//17-04-22 特殊字符也当做精灵处理了

			//如果超过了最大宽度

			if (x >= m_max_w - space_w)
			{
				if (x >= x_max)
					x_max = x;
				x = 0;
				y += dh;
				m_h += dh;
			}

			switch (buffer[i])
			{
			case L' ':
			{
				dw = space_w;
				spr = m_canvas->CreateSprite(0, Quad(Vector2(0, 0), Point(dw, dh)), Color::ALPHACOLOR);

			}
			break;
			case L'\t':
			{
				dw = space_w * 8;
				spr = m_canvas->CreateSprite(0, Quad(Vector2(0, 0), Point(dw, dh)), Color::ALPHACOLOR);

			}
			break;
			case L'\n':

				dw = m_max_w - x;
				spr = m_canvas->CreateSprite(0, Quad(Vector2(0, 0), Point(dw, dh)), Color::ALPHACOLOR);//这里的dw,wh改为int会有问题？？？

				break;
			default:
				//这里返回的是复制的spr
				spr = m_canvas->GetCharSprite(m_font_name, m_font_size, buffer[i]);
				if (spr == NULL)
				{
					debug_warn(String(L"Text_imp::SetStringFast: 字符未注册！") + buffer[i]);
					spr = m_canvas->CreateSprite(0, Quad(Vector2(0, 0), Point(dw, dh)));
				}
				spr->SetColor(m_color);
				break;
			}

				
				spr->SetOrder(m_order);
				coor = spr->GetCoor();
				coor->SetPosition((Point(x, y)));
				coor->SetParent(m_coor);

				m_sprites.push_back(spr);

				x += (UINT32)((INT32)(spr->_quad.v[0].a) + (INT32)(spr->GetSize().w));
		}
		if (x > x_max)
			x_max = x;
		m_size.w = x_max;
		m_size.h = y + dh;

		if (m_align_h == TEXT_ALIGN_LEFT && m_align_v == TEXT_ALIGN_TOP)
			return;

		int ah = m_align_h;
		int av = m_align_v;

		m_align_h = TEXT_ALIGN_LEFT;
		m_align_v = TEXT_ALIGN_TOP;

		SetAlignHorizontal(ah);
		SetAlignVertical(av);

		if (_txtOutline[0])
		{
			for (auto& iter : _txtOutline)
			{
				iter->SetString(m_string);
			}
		}
	}

	String Text_imp::GetString()
	{
		return m_string;
	}

	void Text_imp::RegisterString(const String& str)
	{
		m_canvas->RegisterString(m_font_name, m_font_size, str);
	}

	Coor* DND::Text_imp::GetCoor()
	{
		return m_coor;
	}

	void Text_imp::SetOrder(int order)
	{
		m_order = order;
		list<Sprite*>::iterator itor;
		for (itor = m_sprites.begin(); itor != m_sprites.end(); ++itor)
		{
			(*itor)->SetOrder(order);
		}
	}

	int Text_imp::GetOrder()
	{
		return m_order;
	}

	bool Text_imp::IsPickup()
	{
		//计算文本围成的最大矩形，然后将鼠标转换到相应坐标系
		if (m_sprites.size() == 0)
			return false;
		
		auto iter = m_sprites.begin();
		float h = (float)m_font_size;

		Coor* coor = (*iter)->GetCoor();
		float x1 = ((coor->GetPosition() - m_offset).a);
		float x2 = x1;
		for (; iter != m_sprites.end(); ++iter)
		{
			Sprite* spr = *iter;
			x2 = max(x2, (spr->GetCoor()->GetPosition() - m_offset).a + m_font_size);
		}
		Point mouse = Game::Get()->input->GetMousePosition();
		Vector2 fmouse = m_coor->WorldToThis(mouse);

		return Math::TestCollisionDotInRect(fmouse, Vector4(x1, 0.0f, x2, h));
	}

	void Text_imp::SetAlignHorizontal(int align)
	{
		if (m_align_h == align)
			return;
		switch (align)
		{
		case TEXT_ALIGN_LEFT:
			if (m_align_h == TEXT_ALIGN_HCENTER)
				_sprites_offset(Vector2(m_max_w / 2.0f, 0));
			else
				_sprites_offset(Vector2(m_size.w / 1.0f, 0));
			break;
		case TEXT_ALIGN_HCENTER:
			if (m_align_h == TEXT_ALIGN_LEFT)
				_sprites_offset(Vector2(m_size.w / -2.0f, 0));
			else
				_sprites_offset(Vector2(m_size.w / 2.0f, 0));
			break;
		case TEXT_ALIGN_RIGHT:
			if (m_align_h == TEXT_ALIGN_LEFT)
				_sprites_offset(Vector2(m_size.w / -1.0f, 0));
			else
				_sprites_offset(Vector2(m_size.w / -2.0f, 0));
			break;
		default:
			break;
		}
		m_align_h = align;
	}

	void Text_imp::SetAlignVertical(int align)
	{
		if (m_align_v == align)
			return;
		switch (align)
		{
		case TEXT_ALIGN_TOP:
			if (m_align_v == TEXT_ALIGN_VCENTER)
				_sprites_offset(Vector2(0, m_size.h / 2.0f));
			else
				_sprites_offset(Vector2(0, m_size.h / 1.0f));
			break;
		case TEXT_ALIGN_VCENTER:
			if (m_align_v == TEXT_ALIGN_TOP)
				_sprites_offset(Vector2(0, m_size.h / -2.0f));
			else
				_sprites_offset(Vector2(0, m_size.h / 2.0f));
			break;
		case TEXT_ALIGN_BUTTOM:
			if (m_align_v == TEXT_ALIGN_VCENTER)
				_sprites_offset(Vector2(0, m_size.h / -2.0f));
			else
				_sprites_offset(Vector2(0, m_size.h / -1.0f));
			break;
		default:
			break;
		}
		m_align_v = align;
	}

	void Text_imp::SetColor(Color color)
	{
		Sprite* spr = NULL;
		list<Sprite*>::iterator itor;
		for (itor = m_sprites.begin(); itor != m_sprites.end(); ++itor)
		{
			spr = *itor;
			if (spr->_color[0].Get() != Color::ALPHACOLOR)
				spr->SetColor(color);
		}
		m_color = color;
	}

	Color Text_imp::GetColor()
	{
		return m_color;
	}



	void Text_imp::SetPitchRow(float row /*= 1.0f*/)
	{
		m_pitch_row = row;
	}

	void Text_imp::SetMaxW(unsigned w)
	{
		m_max_w = w;
	}

	unsigned Text_imp::GetH()
	{
		return m_h;

	}

	Sprite* Text_imp::GetPickup(unsigned& n)
	{
		//0为第一个字，精灵返回空代表没有
		n = 0;
		list<Sprite*>::iterator itor;
		for (itor = m_sprites.begin(); itor != m_sprites.end(); ++itor, ++n)
		{

			if ((*itor)->IsPickup())
			{
				return (*itor);
			}

		}
		n = -1;
		return 0;
	}


	Sprite* Text_imp::GetSprite(unsigned n)
	{
		unsigned i = 0;
		list<Sprite*>::iterator itor;
		for (itor = m_sprites.begin(); itor != m_sprites.end(); ++itor, ++i)
		{
			if (i == n)
				return *itor;
		}

		return 0;
	}


	UINT32 Text_imp::GetSpriteNum()
	{
		return m_sprites.size();
	}

	Vector2 Text_imp::GetEndPosition()
	{
		if (m_sprites.empty())
			return Vector2(0, (float)(m_size.h));
		Sprite* spr = m_sprites.back();
		return spr->GetCoor()->GetPosition() + spr->_quad.v[2];
	}

	void Text_imp::SetOutLine(bool open)
	{
		_showOutLine = open;
		if (!open)
			return;
		if (_txtOutline[0] == NULL)
		{
			//描边
			for (auto& iter : _txtOutline)
			{
				iter = this->Clone();
				iter->SetOrder(m_order - 1);
				iter->GetCoor()->SetParent(m_coor);
			}	
		}
		SetOutLineSize(_outlineSize);
	}


	void Text_imp::SetOutLineSize(UINT32 size)
	{
		_outlineSize = size;
		float outline = (float)size;
		if (_txtOutline[0])
		{
			_txtOutline[0]->GetCoor()->SetPosition(Vector2(outline, 0));
			_txtOutline[1]->GetCoor()->SetPosition(Vector2(-outline, 0));
			_txtOutline[2]->GetCoor()->SetPosition(Vector2(0, outline));
			_txtOutline[3]->GetCoor()->SetPosition(Vector2(0, -outline));
			_txtOutline[4]->GetCoor()->SetPosition(Vector2(outline, outline));
			_txtOutline[5]->GetCoor()->SetPosition(Vector2(outline, -outline));
			_txtOutline[6]->GetCoor()->SetPosition(Vector2(-outline, outline));
			_txtOutline[7]->GetCoor()->SetPosition(Vector2(-outline, -outline));
		}
		
	}

	void Text_imp::SetOutLineColor(Color color)
	{
		_outlineColor = color;
		if (_txtOutline[0])
		{
			for (auto& iter : _txtOutline)
			{
				iter->SetColor(color);
			}
		}
		
	}

	void Text_imp::SetFontSize(UINT32 size)
	{
		String str = m_string;
		m_font_size = size;
		m_string = L"";
		SetString(str);
		SetOrder(m_order);
		//描边属性不能复制，否则会造成递归
	}

	UINT32 Text_imp::GetFontSize()
	{
		return m_font_size;
	}

	DND::Text* Text_imp::Clone()
	{
		if (m_canvas)
		{
			Text_imp* text = dynamic_cast<Text_imp*>(m_canvas->CreateText(m_font_name, m_font_size));
			text->m_color = m_color;
			text->m_coor = m_coor->Clone();
			text->m_align_h = m_align_h;
			text->m_align_v = m_align_v;
			text->m_max_w = m_max_w;
			text->m_pitch_row = m_pitch_row;
			text->SetString(m_string);
			text->SetOrder(m_order);
			//描边属性不能复制，否则会造成递归
			
			
			return text;
		}
		else
		{
			return NULL;
		}
	}

	Text_imp::Text_imp(const String& name, unsigned font_size)
	{
		m_font_name = name;
		m_font_size = font_size;
		m_canvas = NULL;
		m_color = Color::WHITE;
		m_align_h = TEXT_ALIGN_LEFT;
		m_align_v = TEXT_ALIGN_TOP;
		m_string = L"";
		m_pitch_row = 1.0f;
		m_max_w = -1;
		m_order = 0;
		m_h = 0;
		_showOutLine = false;
		for (auto& iter : _txtOutline)
		{
			iter = NULL;
		}
		_outlineSize = 1;

	}


	

	Text::~Text()
	{

	}

	Text_imp::~Text_imp()
	{
		Sprite* spr = NULL;
		list<Sprite*>::iterator itor;
		for (itor = m_sprites.begin(); itor != m_sprites.end(); ++itor)
		{
			spr = *itor;
			delete spr;
		}

		if (_txtOutline[0])
		{
			for (auto& iter : _txtOutline)
			{
				delete iter;
			}
		}

		if (m_coor)
		{
			delete m_coor;
		}
	}

	void Text_imp::_sprites_offset(Vector2 offset)
	{
		Sprite* spr = NULL;
		list<Sprite*>::iterator itor;
		for (itor = m_sprites.begin(); itor != m_sprites.end(); ++itor)
		{
			spr = *itor;
			spr->GetCoor()->SetPosition(spr->GetCoor()->GetPosition() + offset);
		}
		m_offset = m_offset + offset;
	}

}

