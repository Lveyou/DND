//////////////////////////////////////////////////////////////////////////
//name:		DNDGUIEditBoxSprite
//author:	Lveyou
//date:		17-09-24

//other:

//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_EDIT_BOX_SPRITE_H_
#define _DND_GUI_EDIT_BOX_SPRITE_H_



#include "DNDDLL.h"
#include "DNDSprite.h"
#include "DNDGUI.h"

namespace DND
{
	class DLL_API GUIEditBoxSprite : public EditBox
	{
	public:
		static GUIEditBoxSprite* Create(Sprite* spr);
	private:
		GUIEditBoxSprite();
		virtual void RunRender() override;
		virtual bool TestCollision() override;
		virtual void OnChar(wchar_t ch) override;
		virtual void OnBack() override;
		Sprite* m_spr;
	};
}


#endif