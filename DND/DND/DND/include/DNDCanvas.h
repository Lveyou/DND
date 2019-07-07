//////////////////////////////////////////////////////////////////////////
//name:		DNDCanvas
//author:	Lveyou
//date:		17-08-11

//other:
//17-08-11: 2d���� - Lveyou
//17-08-11: ����һ�����飬�ض���ע����ͼ��
//17-08-15: ������ͷŷ�ʽ��Ҫ����
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_CANVAS_H_
#define _DND_CANVAS_H_


#include "DNDDLL.h"
#include "DNDImage.h"

const UINT32 DND_SHADER_NORMAL = 0;
const UINT32 DND_SHADER_OVERLAY = 1;//����
const UINT32 DND_SHADER_DARKEN = 2;//�䰵
const UINT32 DND_SHADER_CLOLOR_DODGE = 3;//��ɫ����
const UINT32 DND_SHADER_WATER = 4;//ˮ��

namespace DND
{
	class Sprite;
	class Text;
	class Coor;
	class Sprite9;
	class Tile;
	class DLL_API Canvas
	{
	public:
		static Canvas* Create(INT32 order, bool mipmap = false);
		//�Զ�ע��ͼ��
		virtual Sprite* CreateSprite(const String& img_name) = 0;
		virtual Sprite* CreateSprite(const Image* img) = 0;
		virtual Sprite* CreateSprite(UINT32 img_ID,const Quad& quad, Color color = Color::WHITE) = 0;
		virtual Sprite* CreateSprite(UINT32 img_ID, bool center = true, Color color = Color::WHITE) = 0;
		virtual Sprite9* CreateSprite9(const Image* img, const Rect& xyxy, Color color = Color::WHITE) = 0;
		virtual Sprite* GetCharSprite(const String& name, unsigned font_size, wchar_t ch) = 0;
		
		//ע��ͼ��ȫ��
		virtual void RegisterImageAll(UINT32 img_ID, const Image* img) = 0;
		//����id
		virtual UINT32 RegisterImageAll(const Image* img) = 0;
		//�滻ͼ��ȫ��
		virtual void ReplaceImageAll(UINT32 img_ID, const Image* img) = 0;
		//�����滻ͼ��ȫ��
		virtual void ReplaceImageAllFast(UINT32 img_ID, const Image* img) = 0;
		//ע��ͼ�񲿷�
		virtual void RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect) = 0;
		//ע��ͼ�񲿷֣��Զ�����id
		virtual UINT32 RegisterImageRect(const Image* img, const Rect& rect) = 0;
		//������ͼ��ע��ͼ�񲿷�
		virtual void RegisterImageRect(UINT32 register_ID, UINT32 form_ID, const Rect& rect) = 0;
		//����id
		virtual UINT32 RegisterImageRect(unsigned form_ID, const Rect& rect) = 0;

		virtual const Image* GetImage() = 0;
		virtual bool IsSetImage() = 0;
		virtual Coor* GetCoor() = 0;
		virtual Rect GetImageRect(UINT32 img_ID) = 0;
		//Text
		virtual void RegisterString(const String& name, unsigned font_size, const String& str) = 0;
		virtual Text* CreateText(const String& name, unsigned font_size) = 0;

		//����shader
		virtual void SetShader(UINT32 type = DND_SHADER_NORMAL) = 0;

		virtual UINT32 GetOnGUISpriteNumber() = 0;

		//ֱ������ ����֮���ʡ��ע����ʵ��ע�Ჽ�裬��Ϊͬ���Ĵ���˳�򣬷��ص�id��ʼ����ͬ��
		//ֱ�����õ� ��������tex�Ĵ�СΪͼ���С�����ܻ�С��Ĭ��ֵ
		virtual bool SetImage(const String& img_name, const String& rects) = 0;
		virtual void SaveImageRects(const String& rects) = 0;

		//�����Ƿ�����ͼ��ע�ᣨ���棺������ײ�ʵ�֣�������ã�
		virtual void SetSkipRegister(bool skip) = 0;
	};



}


#endif