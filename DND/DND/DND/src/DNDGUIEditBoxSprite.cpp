#include "DNDGUIEditBoxSprite.h"
#include "DNDDebug.h"

namespace DND
{

	EditBoxSprite::EditBoxSprite()
	{
		
	}

	EditBoxSprite* EditBoxSprite::Create(Sprite* spr)
	{
		EditBoxSprite* ret = new EditBoxSprite;
		ret->m_spr = spr;
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
