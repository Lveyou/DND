//////////////////////////////////////////////////////////////////////////
//name:		DNDTexture
//author:	Lveyou
//date:		17-07-26

//other:
//17-08-11: 真正的d3d纹理 - Lveyou
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_TEXTURE_H_
#define _DND_TEXTURE_H_

#include <d3d11.h>
#include "DNDImage.h"
#include "DNDUser.h"
#include <unordered_map>
using namespace std;

namespace DND
{
	const int TEXTURE_MIPMAP_LEVELS = 8;
	const int TEXTURE_MOST_DETAILED_MIP = 0;
	class Texture
	{
	public:
		Texture(bool mipmap, UINT32 size = 1024);
		void SetImage(const Image* img);
		void AddImageRect(UINT32 ID, const Image* img, const Rect& rect);
		void ReplaceImageRect(UINT32 ID, const Image* img, const Rect& rect);
		void ReplaceImageRectFast(UINT32 ID, const Image* img, const Rect& rect);
		void AddImageRect(UINT32 register_ID, UINT32 form_ID, const Rect& rect);
		float GetTu(UINT32 image_rect_ID, UINT32 index);
		float GetTv(UINT32 image_rect_ID, UINT32 index);
		//一次性返回所有uv（size变化会导致 uv错乱）
		void GetUV(UINT32 image_rect_ID, Vector2* uv);
		
		ID3D11Texture2D* _texture;
		ID3D11ShaderResourceView* _shaderResourceView;
		UINT32 _size;
		
		unordered_map<UINT32, Rect> _imageRects;
		Point _lastAdd;//上一个插入点

		bool _find_xy(const Size& size, Point& xy);

		void _add_img(const Image* img);
		void _add_xy(const Image* img,const Rect& rect, const Point& tar_xy);//先加入 m_img 再加入 tex
	
		void _create_texture2d();
		void _release_texture2d();
		void _create_view();
		void _release_view();

		~Texture();
		Image* _img;
		Image* _imgTemp;

		bool _bMipmap;
	};
}

#endif