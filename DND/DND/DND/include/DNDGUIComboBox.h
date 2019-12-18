//////////////////////////////////////////////////////////////////////////
//name:		DNDGUIComboBox
//author:	Lveyou
//date:		18-10-23

//other:
//18-10-23: 组合框（下拉框）
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_COMBOBOX_H_
#define _DND_GUI_COMBOBOX_H_

#include "DNDDLL.h"
#include "DNDGUI.h"
#include "DNDSprite.h"
#include "DNDText.h"
#include "DNDGUIButtonSprite3.h"
#include "DNDGUISprite9.h"

namespace DND
{


	class DLL_API ComboBox
	{
	public:
		static ComboBox* Create(Sprite* top, Sprite* over, Sprite9* under, ButtonSprite3* right, Text* txt, UINT32 dy = 0);
		void PushBack(const String& str);
		void PushFront(const String& str);
		int Run();
		String GetCur();
		int GetCurNumber();
		void SetCur(int i);
		Coor* GetCoor();
		ComboBox* Clone();

		ButtonSprite3* GetRight() { return _btnRight; }
	private:
		//////////////////////////////////////////////////////////////////////////
		Sprite* _sprTop;//top
		ButtonSprite3* _btnRight;
		Sprite9* _sprUnder;
		Sprite* _sprOver;
		Text* _txt;
		UINT32 _dy;
		int _cur;

		void* _listItem;

		Coor* _coor;
		ComboBox();

	};

}


#endif
