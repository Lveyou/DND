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
		RegisterString(str);
		SetStringFast(str);
	}

	void Text_imp::SetStringFast(const String& str)
	{
		//��������źö��Σ��������ֻ��ͳһƫ������
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
		//���λ��
		unsigned x = 0;
		unsigned x_max = 0;
		unsigned y = 0;
		//����ϴ����о��� Ϊ �ѷ���
		Sprite* spr2 = NULL;
		list<Sprite*>::iterator itor;
		for (itor = m_sprites.begin(); itor != m_sprites.end(); ++itor)
		{
			spr2 = *itor;
			if (spr2)
			{
				spr2->GetCanvas()->DeleteSprite(spr2);
			}
		}
		m_sprites.clear();
		INT32 dw, dh;
		unsigned space_w = (m_font_size >> 2);
		dh = static_cast<int>(m_font_size * m_pitch_row);
		for (int i = 0; buffer[i]; ++i)
		{
			//17-04-22 �����ַ�Ҳ�������鴦����

			//���������������

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
			case L'\n':

				dw = m_max_w - x;
				spr = m_canvas->CreateSprite(0, Quad(Vector2(0, 0), Point(dw, dh)), Color::ALPHACOLOR);//�����dw,wh��Ϊint�������⣿����

				break;
			default:



				//���ﷵ�ص��Ǹ��Ƶ�spr��ͬʱҲ�����ӵ� ���� �� all��
				spr = m_canvas->GetCharSprite(m_font_name, m_font_size, buffer[i]);
				if (spr == NULL)
				{
					//Debug::Instance()->Write_Line(String(L"Set_String ��ĳ�ַ�δע�ᣡ") + buffer[i]);
					assert(spr && L"Set_String ��ĳ�ַ�δע�ᣡ");
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
		Sprite* spr = NULL;
		list<Sprite*>::iterator itor;
		itor = m_sprites.begin();
		if (itor == m_sprites.end())
			return false;
		spr = *itor;
		if (!spr)
			return false;
		
		unsigned h = m_font_size;

		Coor* coor = spr->GetCoor();
		INT32 x1 = (INT32)((coor->GetPosition() - m_offset).a);
		INT32 x2;
		for (; itor != m_sprites.end(); ++itor)
		{
			spr = *itor;
			x2 = INT32((spr->GetCoor()->GetPosition() - m_offset).a + m_font_size);
		}
		Point mouse = Game::Get()->input->GetMousePosition();
		
		return Math::TestCollisionDotInRect(mouse, Rect(Point(x1, 0), Point(x2, h)));
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
		//0Ϊ��һ���֣����鷵�ؿմ���û��
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
			spr->GetCanvas()->DeleteSprite(spr);
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
