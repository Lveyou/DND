//////////////////////////////////////////////////////////////////////////
//name:		DNDText
//author:	Lveyou
//date:		17-08-15

//other:
//17-08-15: 文本控件 Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_TEXT_H_
#define _DND_TEXT_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include "DNDCoor.h"
#include "DNDColor.h"

namespace DND
{
	const int TEXT_ALIGN_LEFT = 0;
	const int TEXT_ALIGN_HCENTER = 1;
	const int TEXT_ALIGN_RIGHT = 2;

	const int TEXT_ALIGN_BUTTOM = 2;
	const int TEXT_ALIGN_VCENTER = 1;
	const int TEXT_ALIGN_TOP = 0;

	class Sprite;
	class DLL_API Text
	{
	public:
		virtual void Render() = 0;
		virtual void RenderFrame() = 0;
		virtual void SetString(const String& str) = 0;
		virtual void SetStringFast(const String& str) = 0;
		virtual void RegisterString(const String& str) = 0;
		virtual String GetString() = 0;
		virtual Coor* GetCoor() = 0;
		virtual Color GetColor() = 0;
		virtual void SetOrder(float order) = 0;
		virtual float GetOrder() = 0;
		virtual bool IsPickup() = 0;
		virtual void SetAlignHorizontal(int align = TEXT_ALIGN_LEFT) = 0;
		virtual void SetAlignVertical(int align = TEXT_ALIGN_TOP) = 0;
		virtual void SetColor(Color color) = 0;
		virtual void SetColorAlpha(float a) = 0;
		virtual void SetColorAlpha(DWORD a) = 0;
		virtual void SetPitchRow(float row = 1.0f) = 0;
		virtual void SetMaxW(unsigned w) = 0;
		virtual unsigned GetH() = 0;
		//警告：下面两个函数返回的精灵，可能会马上失效
		virtual Sprite* GetPickup(unsigned& n) = 0;
		virtual Sprite* GetSprite(unsigned n) = 0;//
		virtual UINT32 GetSpriteNum() = 0;//返回精灵数量
		virtual Vector2 GetEndPosition() = 0;//获取结束位置
		//设置描边，比较耗时
		virtual void SetOutLine(bool open) = 0;
		virtual void SetOutLineSize(UINT32 size) = 0;
		virtual void SetOutLineColor(Color color) = 0;

		//较为耗时操作
		virtual void SetFontSize(UINT32 size) = 0;
		virtual UINT32 GetFontSize() = 0;

		virtual Text* Clone() = 0;
		
		virtual ~Text() = 0;
	};



}

#endif