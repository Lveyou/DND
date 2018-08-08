//////////////////////////////////////////////////////////////////////////
//name:		DNDGUIButtonSpriteScale
//author:	Lveyou
//data:		17-08-17

//other:
//17-08-17: �������Ű�ť
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_BUTTON_SPRITE_SCALE_H_
#define _DND_GUI_BUTTON_SPRITE_SCALE_H_

#include "DNDDLL.h"
#include "DNDGUI.h"
#include "DNDSprite.h"

namespace DND
{


	class DLL_API ButtonSpriteScale : public Control
	{
	public:
		//���飬������Сֵ���������ֵ�������ٶ�
		static ButtonSpriteScale* Create(Sprite* spr, float min, float max, float dt);
		Sprite* GetSprite();
	private:
		//����ؼ���Ҫ����
		virtual bool _is_pickup() override;
		virtual void _update(State state) override;
		//////////////////////////////////////////////////////////////////////////
		Sprite* _spr;
		float _min;
		float _max;
		float _dt;
		float _cur;

		ButtonSpriteScale();

	};


}


#endif
