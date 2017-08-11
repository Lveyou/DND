#ifndef _DND_TEXTURE_H_
#define _DND_TEXTURE_H_

//name : texture
//note : 真正的d3d纹理
//author : wuganyou
//data : 16-09-08
//other: 此类具有所有 ImageRect信息

#include <d3d11.h>
#include "DNDImage.h"
#include "DNDGeom.h"
#include <map>
using namespace std;

namespace DND
{
	const int TEXTURE_MIPMAP_LEVELS = 8;
	class Texture
	{
	public:
		Texture();
		void Set_Image(const Image* img);
		void Add_Image_Rect(unsigned ID, const Image* img, const Rect& rect);
		void Add_Image_Rect(unsigned register_ID, unsigned form_ID, const Rect& rect);
		//void Reflush_ImageRect(unsigned ID, const Image* img, const Vector4& rect);
		float Get_Tu(unsigned image_rect_ID, unsigned index);
		float Get_Tv(unsigned image_rect_ID, unsigned index);
		ID3D11Texture2D* m_texture;
		ID3D11ShaderResourceView* m_shader_resource_view;
		unsigned m_size;
		
		map<unsigned, Rect> m_image_rects;

		bool _find_xy(const Size& size, Point& xy);

		void _add_img(const Image* img);
		void _add_xy(const Image* img,const Rect& rect, const Point& tar_xy);//先加入 m_img 再加入 tex
		//Image* _to_img();
		void _create_texture2d();
		void _release_texture2d();
		void _create_view();
		void _release_view();

		~Texture();
		Image* m_img;
		Image* m_img_temp;
	};
}

#endif