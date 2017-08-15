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