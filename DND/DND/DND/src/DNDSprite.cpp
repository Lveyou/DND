#include "DNDSprite.h"
#include "DNDCoor_imp.h"
#include "DNDSystem_imp.h"
#include "DNDInput.h"
#include "DNDMath.h"
#include "DNDDebug.h"
#include "DNDDirectX_imp.h"
#include "DNDCamera_imp.h"

namespace DND
{
	void Sprite::Render()
	{
		m_show = true;
	}

	bool Sprite::Is_Pickup()
	{
		//自身构成的 两个三角形 与鼠标形成的射线 相交(dir 为射线的方向，在此处就是 相机的方向)
		System_imp* sys = System_imp::Get_Instance();
		Input* input = Input::Get_Instance();

		// 鼠标位置 从窗口转换到 相机
		Vector3 mouse = sys->Get_Point_Window_To_Camera(input->Get_Mouse_Position());
		//再转到 世界
		mouse = sys->Get_Point_Camera_To_World(mouse);

		Vector3 orig;
		Vector3 dir;
		if (!sys->Is_Camera_Orthographic())
		{
			orig = sys->Get_Camera_Position();
			dir = mouse - orig;
		}
		else
		{
			orig = mouse + Vector3(0, 0, CAMERA_2D_EYE_Z);
			dir = Vector3(0, 0, 1.0f);
		}


		/*Debug::Instance()->Write_Line(String::Format(64, L"%f,%f,%f", mouse.a, mouse.b, mouse.c));
		Debug::Instance()->Write_Line(String::Format(64, L"%f,%f,%f", orig.a, orig.b, orig.c));*/

		//orig = Vector3(0,0,0);

		sys->Render_Line(orig, mouse, Color::YELLOW);

		Vector3 v[4];
		v[0] = m_quad.v[0].Get_Position().To_Vector3(m_z[0]);
		v[1] = m_quad.v[1].Get_Position().To_Vector3(m_z[1]);
		v[2] = m_quad.v[2].Get_Position().To_Vector3(m_z[2]);
		v[3] = m_quad.v[3].Get_Position().To_Vector3(m_z[3]);
		if (m_coor)
		{
			v[0] = m_coor->Get_Position_This_To_World(v[0]);
			v[1] = m_coor->Get_Position_This_To_World(v[1]);
			v[2] = m_coor->Get_Position_This_To_World(v[2]);
			v[3] = m_coor->Get_Position_This_To_World(v[3]);
		}

		if (Math::Test_Collision_Ray_And_Triangle(orig, dir, v[0], v[1], v[2]) ||
			Math::Test_Collision_Ray_And_Triangle(orig, dir, v[0], v[2], v[3]))
		{
			return true;
		}
		else
			return false;
	}

	void Sprite::Render_Frame()
	{
		//先装换到 世界坐标系 (再转到 相机坐标系。。) 
		Vector3 v[4];
		v[0] = m_quad.v[0].Get_Position().To_Vector3(m_z[0]);
		v[1] = m_quad.v[1].Get_Position().To_Vector3(m_z[1]);
		v[2] = m_quad.v[2].Get_Position().To_Vector3(m_z[2]);
		v[3] = m_quad.v[3].Get_Position().To_Vector3(m_z[3]);
		if (m_coor)
		{
			v[0] = m_coor->Get_Position_This_To_World(v[0]);
			v[1] = m_coor->Get_Position_This_To_World(v[1]);
			v[2] = m_coor->Get_Position_This_To_World(v[2]);
			v[3] = m_coor->Get_Position_This_To_World(v[3]);
		}

		System_imp* sys = System_imp::Get_Instance();
		/*v[0] = sys->Get_Point_World_To_Camera(v[0]);
		v[1] = sys->Get_Point_World_To_Camera(v[1]);
		v[2] = sys->Get_Point_World_To_Camera(v[2]);
		v[3] = sys->Get_Point_World_To_Camera(v[3]);*/
		Color color = Color::YELLOW;
		if (Is_Pickup())
			color = Color::PURPLE;

		sys->Render_Line(v[0], v[1], color);
		sys->Render_Line(v[1], v[2], color);
		sys->Render_Line(v[2], v[3], color);
		sys->Render_Line(v[3], v[0], color);
		sys->Render_Line(v[2], v[0], color);

	}

	Coor* Sprite::Get_Coor()
	{
		return m_coor;
	}

	void Sprite::Set_Order(int order)
	{
		m_order = order;
	}

	void Sprite::Set_Z(const Vector4& z)
	{
		m_z = z;
	}

	Size Sprite::Get_Size()
	{
		return Size(m_quad.v[2].Get_Point().x - m_quad.v[0].Get_Point().x,
			m_quad.v[3].Get_Point().y - m_quad.v[1].Get_Point().y);
	}

	void Sprite::Set_Color(Color color)
	{
		m_color[0] = color;
		m_color[1] = color;
		m_color[2] = color;
		m_color[3] = color;
	}

	Canvas* Sprite::Get_Canvas()
	{
		return m_canvas;
	}

	Sprite::~Sprite()
	{
		//调用 Delete 删除
		if (m_coor)
			delete m_coor;
	}

	Sprite* Sprite::Clone()
	{
		Sprite* spr = m_canvas->Create_Sprite(m_image_rect_ID, m_quad, Color::WHITE);
		spr->m_order = m_order;
		//spr->m_canvas = b.m_canvas;
		if (m_coor)
			spr->m_coor = m_coor->Clone();
		else
			m_coor = 0;
		//spr->m_quad = m_quad;
		spr->m_color[0] = m_color[0];
		spr->m_color[1] = m_color[1];
		spr->m_color[2] = m_color[2];
		spr->m_color[3] = m_color[3];
		spr->m_z = m_z;
		//spr->m_image_rect_ID = m_image_rect_ID;
		spr->m_show = false;//07-04-27 改为了 false
		spr->m_dead = false;

		return spr;
	}

	void Sprite::Delete()
	{
		m_dead = true;
	}

	Sprite::Sprite()
	{
		m_order = 0;
		m_show = false;
		m_coor = 0;
		m_dead = false;
	}

	/*Sprite::Sprite(const Sprite & b)
	{
		m_order = b.m_order;
		m_canvas = b.m_canvas;
		if (b.m_coor)
			m_coor = b.m_coor->Clone();
		else
			m_coor = 0;
		m_quad = b.m_quad;
		m_color[0] = b.m_color[0];
		m_color[1] = b.m_color[1];
		m_color[2] = b.m_color[2];
		m_color[3] = b.m_color[3];
		m_z = b.m_z;
		m_image_rect_ID = b.m_image_rect_ID;
		m_show = b.m_show;
		m_dead = b.m_dead;


	}*/

}

