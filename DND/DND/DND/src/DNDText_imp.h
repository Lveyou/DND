#ifndef _DND_TEXT_IMP_H_
#define _DND_TEXT_IMP_H_

//name : text_imp
//note : 文本
//author : wuganyou
//data : 16-09-25
//other: 

#include "DNDText.h"
#include "DNDSprite.h"
#include <list>
using namespace std;

namespace DND
{
	class Canvas_imp;
	class Text_imp : public Text
	{
	public:
		virtual void Render() override;
		virtual void RenderFrame() override;
		virtual void SetString(const String& str) override;
		virtual void SetStringFast(const String& str) override;
		virtual String GetString() override;
		virtual void RegisterString(const String& str) override;
		virtual Coor* GetCoor() override;
		virtual Color GetColor() override;
		virtual void SetOrder(int order) override;
		virtual int GetOrder() override;
		virtual bool IsPickup() override;
		virtual void SetAlignHorizontal(int align = TEXT_ALIGN_LEFT) override;
		virtual void SetAlignVertical(int align = TEXT_ALIGN_TOP) override;
		virtual void SetColor(Color color) override;
		virtual void SetPitchRow(float row = 1.0f) override;
		virtual void SetMaxW(unsigned w) override;
		virtual unsigned GetH() override;
		virtual Sprite* GetPickup(unsigned& n) override;
		virtual Sprite* GetSprite(unsigned n) override;
		virtual UINT32 GetSpriteNum() override;
		virtual Vector2 GetEndPosition() override;
		virtual void SetOutLine(bool open) override;
		virtual void SetOutLineSize(UINT32 size) override;
		virtual void SetOutLineColor(Color color) override;
		virtual void SetFontSize(UINT32 size) override;
		virtual UINT32 GetFontSize() override;
		virtual Text* Clone() override;
		
		
		Text_imp(const String& name, unsigned font_size);
		~Text_imp();
		String m_font_name;
		unsigned m_font_size;

		Color m_color;
		int m_order;
		//所用到的Sprite副本，在Set_String调用时刷新
		list<Sprite*> m_sprites;

		Canvas_imp* m_canvas;
		//位置 ，有所字符精灵 是它的子节点
		Coor* m_coor;

		int m_align_h;
		int m_align_v;

		Size m_size;
		String m_string;
		float m_pitch_row;//行间距
		unsigned m_max_w;//最大像素宽度
		unsigned m_h;
		void _sprites_offset(Vector2 offset);
		Vector2 m_offset;
		//描边的8个
		bool _showOutLine;
		Text* _txtOutline[8];
		UINT32 _outlineSize;
		Color _outlineColor;
	};
}



#endif