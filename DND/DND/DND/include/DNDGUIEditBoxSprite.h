//////////////////////////////////////////////////////////////////////////
//name:		DNDGUIEditBoxSprite
//author:	Lveyou
//date:		17-09-24

//other:
//1.由于需要定位光标，改为所有Edit都有txt 19-04-28

//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_EDIT_BOX_SPRITE_H_
#define _DND_GUI_EDIT_BOX_SPRITE_H_



#include "DNDDLL.h"
#include "DNDSprite.h"
#include "DNDGUI.h"

namespace DND
{
	class DLL_API EditBoxSprite : public EditBox
	{
	public:
		static EditBoxSprite* Create(Sprite* spr, Text* txt);
		void Push(wchar_t ch);
		Sprite* GetSprite() { return _spr; }
	private:
		EditBoxSprite();
		virtual void RunRender() override;
		virtual bool TestCollision() override;
		virtual void OnChar(wchar_t ch) override;
		virtual void OnBack() override;

		Sprite* _spr;
	};
}


#endif