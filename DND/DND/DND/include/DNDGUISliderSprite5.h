//////////////////////////////////////////////////////////////////////////
//name:		DNDGUISliderSprite5
//author:	Lveyou
//date:		19-02-21

//other:
//19-02-21: head��body��tail��under��slider��ɵ� ������
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_GUI_SLIDER_SPRITE_5_H_
#define _DND_GUI_SLIDER_SPRITE_5_H_

#include "DNDDLL.h"
#include "DNDSprite.h"

namespace DND
{


	class DLL_API SliderSprite5
	{
	public:
		static SliderSprite5* Create(
			Sprite* head, Sprite* body, Sprite* tail,
			Sprite* under, Sprite* slider, int length, int max, Vector2 offset, bool r = true, bool filp = false);
		Coor* GetCoor();

		void Run();
		float GetCur();
		void SetCur(float k);

		SliderSprite5* Clone();
	private:
		Sprite* _spr[5];
		Coor* _coor;
		SliderSprite5();

		float _cur;// 0 - 1.0f
		int _max;//������Χ
		bool _seleted;//�Ƿ�ѡ��
		bool _r;//�Ƿ����
		Vector2 _offset;//����������ƫ��

		float _underSize;
	};

}


#endif
