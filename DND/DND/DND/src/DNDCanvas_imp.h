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
		//Sprites �� ����˳��С���Ȼ��� ������ sprite
		multimap<int, Sprite*> _sprites;

		
		//���㻺��
		ID3D11Buffer* _bufferVertex;
		//���㻺��(�ڴ���)��С
		unsigned _vertexSize;
		//�����ڴ���
		Vertex2D* _vertexs;
		//����ϵ
		Coor* _coor;
		//��ͼ
		Texture* _tex;

		//����ϵͳע��idyon
		unsigned _systemUseID;
		//�ַ������
		vector<CharSpriteNode> _charSprites;
	};

}