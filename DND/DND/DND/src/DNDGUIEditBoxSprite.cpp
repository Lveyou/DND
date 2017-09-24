#include "DNDGUIEditBoxSprite.h"
#include "DNDDebug.h"

namespace DND
{

	DND::GUIEditBoxSprite::GUIEditBoxSprite(Sprite* spr)
	{
		m_spr = spr;
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
		m_string.Pop();
	}

}
