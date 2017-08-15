#include "DNDCanvas.h"
#include "DNDCoor.h"
#include "DNDTexture.h"

#include <d3d11.h>
#include <list>
#include <map>
using namespace std;

namespace DND
{
	const UINT32 CANVAS_SYS_ID_START = 1000000000;
	struct Vertex2D;
	struct CharSpriteNode
	{
		Sprite* spr;
		WCHAR ch;
		String name;
		unsigned size;
		bool operator==(const CharSpriteNode& b);
	};
	class Canvas_imp : public Canvas
	{
	public:
		virtual Sprite* CreateSprite(const String& img_name) override;
		virtual Sprite* CreateSprite(const Image* img) override;
		virtual Sprite* CreateSprite(UINT32 img_ID,const Quad& quad, Color color = Color::WHITE) override;
		virtual Sprite* GetCharSprite(const String& name, unsigned font_size, wchar_t ch) override;
		virtual void DeleteSprite(Sprite* spr) override;
		virtual void RegisterImageAll(UINT32 img_ID, const Image* img) override;
		virtual void RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect) override;
		virtual void RegisterImageRect(UINT32 register_ID, UINT32 form_ID, const Rect& rect) override;
		virtual void RegisterString(const String& name, unsigned font_size, const String& str) override;
		virtual Text* CreateText(const String& name, unsigned font_size) override;

		Canvas_imp(UINT32 order);
		void _render();
		void _update();//m_all_sprites => m_sprites => m_vertexs => m_buffer_vertex
		void _create_vertex_buffer();
		void _release_vertex_buffer();

		UINT32 _order;
		//all_sprite
		list<Sprite*> _allSprite;
		//Sprites 按 绘制顺序（小的先画） 存所有 sprite
		multimap<int, Sprite*> _sprites;

		
		//顶点缓存
		ID3D11Buffer* _bufferVertex;
		//顶点缓存(内存区)大小
		unsigned _vertexSize;
		//顶点内存区
		Vertex2D* _vertexs;
		//坐标系
		Coor* _coor;
		//贴图
		Texture* _tex;

		//用于系统注册idyon
		unsigned _systemUseID;
		//字符精灵表
		vector<CharSpriteNode> _charSprites;
	};

}