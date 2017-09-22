//////////////////////////////////////////////////////////////////////////
//name:		DNDGUIButtonSprite3Text1
//author:	Lveyou
//data:		17-09-22

//other:
//17-09-22: ������һ�ı���������
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_BUTTON_SPRITE_3_TEXT_1_H_
#define _DND_GUI_BUTTON_SPRITE_3_TEXT_1_H_

#include "DNDDLL.h"
#include "DNDGUI.h"
#include "DNDSprite.h"
#include "DNDText.h"

namespace DND
{


	class DLL_API ButtonSprite3Text1 : public Control
	{
	public:
		//���飬������Сֵ���������ֵ�������ٶ�
		static ButtonSprite3Text1* Create(Sprite* normal, Sprite* over, Sprite* down, Text* txt);
		Text* GetText();
		Coor* GetCoor();
	private:
		//����ؼ���Ҫ����
		virtual bool _is_pickup() override;
		virtual void _update(State state) override;
		//////////////////////////////////////////////////////////////////////////
		Sprite* _spr[3];
		Text* _txt;
		Coor* _coor;
		ButtonSprite3Text1();

	};

}


#endif