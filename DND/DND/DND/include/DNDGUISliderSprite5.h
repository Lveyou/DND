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

		//��1.0���ֵ� 0 - N-1 ,�Ӷ���Ӧ����(��Ϊ0����Ӧ)
		void SetN(UINT32 n = 0);

		bool IsFocus() { return _focus == this; }

		Sprite* GetSprite(UINT32 n) { return _spr[n]; }
		void SetOrder(int order);

		SliderSprite5* Clone();
		static SliderSprite5* _focus;
	private:
		Sprite* _spr[5];
		Coor* _coor;
		SliderSprite5();

		float _cur;// 0 - 1.0f
		int _max;//������Χ
		bool _seleted;//�Ƿ�ѡ��
		bool _r;//�Ƿ����
		Vector2 _offset;//����������ƫ��
		UINT32 _n;

		float _underSize;
	};

}


#endif