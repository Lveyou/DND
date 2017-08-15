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
		wchar_t ch;
		String name;
		unsigned size;
		bool operator==(const CharSpriteNode& b);
	};
	class Canvas_imp : public Canvas
	{
	public:
		virtual Sprite* CreateSprite(const String& imgName) override;
		virtual Sprite* CreateSprite(const Image* img) override;
		virtual Sprite* CreateSprite(UINT32 imgID,const Quad& quad, Color color = Color::WHITE) override;
		virtual void RegisterImageAll(UINT32 imgID, const Image* img) override;
		virtual void RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect) override;
		virtual void RegisterImageRect(UINT32 registerID, UINT32 formID, const Rect& rect) override;

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