//////////////////////////////////////////////////////////////////////////
//name:		DNDTexture
//author:	Lveyou
//data:		17-07-26

//other:
//17-08-11: ������d3d���� - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_TEXTURE_H_
#define _DND_TEXTURE_H_

#include <d3d11.h>
#include "DNDImage.h"
#include "DNDUser.h"
#include <map>
using namespace std;

namespace DND
{
	const int TEXTURE_MIPMAP_LEVELS = 8;
	class Texture
	{
	public:
		Texture();
		void SetImage(const Image* img);
		void AddImageRect(UINT32 ID, const Image* img, const Rect& rect);
		void AddImageRect(UINT32 registerID, UINT32 formID, const Rect& rect);
		float GetTu(UINT32 imageRectID, UINT32 index);
		float GetTv(UINT32 imageRectID, UINT32 index);
		ID3D11Texture2D* _texture;
		ID3D11ShaderResourceView* _shaderResourceView;
		UINT32 _size;
		
		map<unsigned, Rect> _imageRects;

		bool _find_xy(const Size& size, Point& xy);

		void _add_img(const Image* img);
		void _add_xy(const Image* img,const Rect& rect, const Point& tarXY);//�ȼ��� m_img �ټ��� tex
	
		void _create_texture2d();
		void _release_texture2d();
		void _create_view();
		void _release_view();

		~Texture();
		Image* _img;
		Image* _imgTemp;
	};
}

#endif