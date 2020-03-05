#include "DNDGUIEditBoxSprite.h"
#include "DNDDebug.h"
#include "DNDText.h"

namespace DND
{

	EditBoxSprite::EditBoxSprite()
	{
		
	}

	EditBoxSprite* EditBoxSprite::Create(Sprite* spr, Text* txt)
	{
		EditBoxSprite* ret = new EditBoxSprite;
		ret->_txt = txt;
		ret->_spr = spr;
		ret->_spr->GetCoor()->SetParent(ret->_txt->GetCoor());
		ret->_spr->SetUI(true);
		
		ret->SetOrder(spr->GetOrder());

		return ret;
	}

	void EditBoxSprite::Push(wchar_t ch)
	{
		OnChar(ch);
	}

	void EditBoxSprite::RunRender()
	{
		_spr->Render();
		_txt->Render();
	}

	bool EditBoxSprite::TestCollision()
	{
		return _spr->IsPickup();
	}

	void EditBoxSprite::OnChar(wchar_t ch)
	{

		m_string = m_string + ch;
	}

	void EditBoxSprite::OnBack()
	{
		if(m_string.GetLength())
			m_string.Pop();
	}

	void EditBoxSprite::SetOrder(float order)
	{
		_spr->SetOrder(order);
		_txt->SetOrder(order - FLT_EPSILON);
	}

}
