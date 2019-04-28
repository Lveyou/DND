#include "DNDGUIEditBoxSprite.h"
#include "DNDDebug.h"
#include "DNDText.h"

namespace DND
{

	EditBoxSprite::EditBoxSprite()
	{
		
	}

	EditBoxSprite* EditBoxSprite::Create(Sprite* spr, Text* txt)
	{
		EditBoxSprite* ret = new EditBoxSprite;
		ret->m_spr = spr;
		ret->m_spr->SetUI(true);
		ret->_txt = txt;
		ret->_txt->GetCoor()->SetParent(ret->m_spr->GetCoor());

		return ret;
	}

	void EditBoxSprite::Push(wchar_t ch)
	{
		OnChar(ch);
	}

	void EditBoxSprite::RunRender()
	{
		m_spr->Render();
	}

	bool EditBoxSprite::TestCollision()
	{
		return m_spr->IsPickup();
	}

	void EditBoxSprite::OnChar(wchar_t ch)
	{

		m_string = m_string + ch;
	}

	void EditBoxSprite::OnBack()
	{
		if(m_string.GetLength())
			m_string.Pop();
	}

}
