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
	const UINT32 CANVAS_TEXT_ID_START = 2000000000;//�ı�ID��ʼ
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

		virtual bool SetImage(const String& img_name, const String& rects) override;
		virtual void SaveImageRects(const String& rects) override;
		virtual bool IsSetImage() override;

		virtual void SetSkipRegister(bool skip) override;

		Canvas_imp(INT32 order, bool mipmap = false);
		void _render();
		void _update();//m_all_sprites => m_sprites => m_vertexs => m_buffer_vertex
		void _create_vertex_buffer();
		void _release_vertex_buffer();

		INT32 _order;
		//all_sprite
		//list<Sprite*> _allSprite;
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

		//���������*4
		UINT32 _renderSprNum;

		//������UI������
		UINT32 _onGUISpr;

		//�Զ�������
		bool _bSetImage;

		//Shader
		UINT32 _shaderType;
	};

}