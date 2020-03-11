#include "DNDCanvas.h"
#include "DNDCoor.h"
#include "DNDTexture.h"
#include "DNDSprite.h"

#include <list>
using namespace std;

namespace DND
{
	
	

	const UINT32 CANVAS_SYS_ID_START = 1000000000;
	const UINT32 CANVAS_TEXT_ID_START = 2000000000;//文本ID起始
	struct Vertex2D;
	class Sprite9;
	class Tile;
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
		virtual Sprite* CreateSprite(UINT32 img_ID, bool center = true, Color color = Color::WHITE) override;
		virtual Sprite9* CreateSprite9(const Image* img, const Rect& xyxy, Color color = Color::WHITE) override;
		virtual Sprite* GetCharSprite(const String& name, unsigned font_size, wchar_t ch) override;
		
	

		virtual void RegisterImageAll(UINT32 img_ID, const Image* img) override;
		virtual UINT32 RegisterImageAll(const Image* img) override;
		virtual void ReplaceImageAll(UINT32 img_ID, const Image* img) override;
		virtual void ReplaceImageAllFast(UINT32 img_ID, const Image* img) override;
		virtual void RegisterImageRect(UINT32 ID, const Image* img, const Rect& rect) override;
		virtual UINT32 RegisterImageRect(const Image* img, const Rect& rect) override;
		virtual void RegisterImageRect(UINT32 register_ID, UINT32 form_ID, const Rect& rect) override;
		virtual UINT32 RegisterImageRect(unsigned form_ID, const Rect& rect) override;
		virtual const Image* GetImage() override;
		virtual Coor* GetCoor() override;
		virtual Rect GetImageRect(UINT32 img_ID) override;
		virtual void RegisterString(const String& name, unsigned font_size, const String& str) override;
		virtual Text* CreateText(const String& name, unsigned font_size) override;

		virtual void SetShader(UINT32 type = DND_SHADER_NORMAL) override;

		virtual UINT32 GetOnGUISpriteNumber() override;
		virtual float GetOnGUISpriteMaxOrder() override;

		virtual bool SetImage(const String& img_name, const String& rects) override;
		virtual void SaveImageRects(const String& rects) override;
		virtual bool IsSetImage() override;
		virtual bool IsAddedText() override;

		virtual void SetSkipRegister(bool skip) override;


		Canvas_imp(INT32 order, bool mipmap = false, UINT32 size = 1024, UINT32 vertex_size = 1024);
		void _render();
		void _update();//m_all_sprites => m_sprites => m_vertexs => m_buffer_vertex
		void _delete_spr();
		void _create_vertex_buffer();
		void _release_vertex_buffer();

		inline void _insert_sprite(Sprite* spr)
		{
			//_sprites.insert(make_pair(spr->_order, spr));
			_sprites.push_back(spr);
		}
		//multimap<int, Sprite*>::iterator _iter;

		INT32 _order;
		//all_sprite
		//list<Sprite*> _allSprite;
		//Sprites 按 绘制顺序（小的先画） 存所有 sprite
		
		
		//multimap<int, Sprite*> _sprites;
		//改为Z-buffer
		list<Sprite*> _sprites;

		//需要删除的精灵
		list<Sprite*> _spritesDeleted;
		//z值的优先级队列（越大越先出，先入先出）【有奇怪的bug……】
		//priority_queue<Sprite*, vector<Sprite*>, SpriteCompareZ> _sprites;

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
		//贴图2（用于shader）
		Texture* _tex2;

		//用于系统注册idyon
		unsigned _systemUseID;
		//字符精灵表
		vector<CharSpriteNode> _charSprites;

		//精灵绘制数*4
		UINT32 _renderSprNum;

		//触碰的UI精灵数
		UINT32 _onGUISpr;
		//最大ui精灵 的order（会屏蔽小order的ui精灵 检测）
		float _orderUISprMin;

		//自定义纹理
		bool _bSetImage;
		bool _bAddedText;//是否注册了新字体
		//Shader
		UINT32 _shaderType;

	};

}