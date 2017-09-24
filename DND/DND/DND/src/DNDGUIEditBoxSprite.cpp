#include "DNDGUIEditBoxSprite.h"
#include "DNDDebug.h"

namespace DND
{

	GUIEditBoxSprite::GUIEditBoxSprite()
	{
		
	}

	GUIEditBoxSprite* GUIEditBoxSprite::Create(Sprite* spr)
	{
		GUIEditBoxSprite* ret = new GUIEditBoxSprite;
		ret->m_spr = spr;
		return ret;
	}

	void GUIEditBoxSprite::RunRender()
	{
		m_spr->Render();
	}

	bool GUIEditBoxSprite::TestCollision()
	{
		return m_spr->IsPickup();
	}

	void GUIEditBoxSprite::OnChar(wchar_t ch)
	{

		m_string = m_string + ch;
	}

	void GUIEditBoxSprite::OnBack()
	{
		if(m_string.GetLength())
			m_string.Pop();
	}

}
