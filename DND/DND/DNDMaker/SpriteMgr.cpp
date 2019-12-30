#include "SpriteMgr.h"



SpriteMgr::SpriteMgr()
{
	_sl._curPack = -1;
}

void SpriteMgr::Init(Maker* maker)
{
	_maker = maker;

	AddCanvas(L"all", _maker->canvas);


	_btnAdd = _maker->GetTempBtnMenu();
	_btnAdd->GetText()->SetString(L"添加画布");

	auto* temp = _maker->canvas->CreateText(L"en", 22);
	temp->SetColor(0xff248e1c);

	_edtAddName = EditBoxSprite::Create(
		_maker->canvas->CreateSprite(0, Quad(Vector2(0, 0), Vector2(101, 32), false), 0xaaffffff),
		temp);
	_edtAddName->SetModeNumber(false);//数字
	_edtAddName->SetModeLetter(true);//字母
	_edtAddName->SetModeSymbol(false);//符号
	_edtAddName->SetModeContrl(false);//控制字符
	_edtAddName->SetModeOther(false);//其余


	_txtNoticeAdd = _maker->canvas->CreateText(L"cn", 18);
	_txtNoticeAdd->SetString(L"选择一个画布，将精灵添加进去");
	_txtNoticeAdd->SetColor(0xff1722bb);
	_txtNoticeAdd->GetCoor()->SetParent(_maker->GetCoorRight());
	_txtNoticeAdd->GetCoor()->SetPosition(Vector2(-15, 10));

	_btnTools[ANCHOR] = _maker->GetTempBtnMenu();
	_btnTools[ANCHOR]->GetText()->SetString(L"设置锚点");
	_btnTools[ANCHOR]->SetMode(Control::RADIO);

	_btnTools[SCALE] = _maker->GetTempBtnMenu();
	_btnTools[SCALE]->GetText()->SetString(L"缩放");
	_btnTools[SCALE]->SetMode(Control::RADIO);

	_btnTools[ROTATE] = _maker->GetTempBtnMenu();
	_btnTools[ROTATE]->GetText()->SetString(L"旋转");
	_btnTools[ROTATE]->SetMode(Control::RADIO);

	_btnTools[STRETCH] = _maker->GetTempBtnMenu();
	_btnTools[STRETCH]->GetText()->SetString(L"形变");
	_btnTools[STRETCH]->SetMode(Control::RADIO);
}

void SpriteMgr::Render(Vector2 start, float h)
{
	UINT32 i = 0;
	const float dx = 100.0f;
	const float dy = 16.0f;
	const float x2 = 400.0f;
	UINT32 h_num = h / dy;

	for (auto& iter : _mapCanvas)
	{
		auto* btn = iter.second->_btn;

		btn->GetCoor()->SetPosition(start + Vector2(i / h_num * dx, i % h_num * dy));
		btn->Run();
		if (btn->IsOpen())
		{
			for (auto& iter2 : _mapCanvas)
			{
				if (iter2.second->_btn != iter.second->_btn)
					iter2.second->_btn->SetOpen(false);
			}
			//logic
			_sl._curPack = i;

		}
		++i;
	}
	_btnAdd->GetCoor()->SetPosition(start + Vector2(0, i % h_num * dy + 30));
	_btnAdd->Run();

	_edtAddName->GetCoor()->SetPosition(start + Vector2(0, i % h_num * dy + 73));
	_edtAddName->Run();

	if (_btnAdd->IsRelease())
	{
		String name = _edtAddName->GetString();
		if (name != L"")
		{
			if (_mapCanvas.find(name) == _mapCanvas.end())
				AddCanvas(name, Canvas::Create(_mapCanvas.size()));
		}
	}

	CanvasPack* cur_canvas = NULL;
	SpriteNode* sprite_node = NULL;
	if (_sl._curPack >= 0)
	{
		auto iter5 = _mapCanvas.begin();
		advance(iter5, _sl._curPack);

		if (iter5 != _mapCanvas.end())
		{
			UINT32 j = 0;
			for (auto& iter2 : iter5->second->_mapSprites)
			{
				auto* btn = iter2.second._btn;

				btn->GetCoor()->SetPosition(start + Vector2(dx + 31 + j / h_num * dx, j % h_num * dy));
				btn->Run();
				if (btn->IsOpen())
				{
					for (auto& iter3 : iter5->second->_mapSprites)
					{
						if (iter3.second._btn != btn)
							iter3.second._btn->SetOpen(false);
					}
					//logic
					iter5->second->_curNode = j;
					cur_canvas = iter5->second;
				}
				++j;
			}

			if (iter5->second->_curNode >= 0)
			{
				auto iter6 = iter5->second->_mapSprites.begin();
				advance(iter6, iter5->second->_curNode);

				if (iter6 != iter5->second->_mapSprites.end())
				{
					sprite_node = &(iter6->second);
					iter6->second._spr->Render();
					iter6->second._spr->RenderFrame();
				}
			}
		}
	}

	//添加到画布按钮
	UINT32 k = 0;
	for (auto& iter7 : _mapCanvas)
	{
		auto* iter8 = iter7.second->_btnAddThis;
		iter8->GetCoor()->SetPosition(start + Vector2(0, k++ * 33.f));
		iter8->Run();
	}

	_txtNoticeAdd->Render();

	//工具一栏
	UINT32 l = 0;
	for (auto& iter9 : _btnTools)
	{
		iter9->GetCoor()->SetPosition(Vector2(l++ * dx + 400, 65));
		iter9->Run();

		if (iter9->IsOpen())
		{
			for (auto& iter10 : _btnTools)
			{
				if (iter9 != iter10)
					iter10->SetOpen(false);
			}
			//logic
			_sl._curTool = l - 1;

		}
	}

	//
	if (_sl._curTool == 0)
	{
		//设置锚点
		if (sprite_node)
		{
			if (_maker->input->KeyUp(KeyCode::MOUSE_L))
			{
				Vector2 pos = sprite_node->_spr->GetCoor()->WorldToThis(_maker->input->GetMousePosition());
				if (sqrt(pos.GetlengthSquared()) < sprite_node->_spr->GetSize().w * 2)
				{
					sprite_node->_spr->SetQuadOffset(-pos);
				}
				
			}
		}
	}
}

void SpriteMgr::AddCanvas(String canvas_name, Canvas* canvas)
{
	

	CanvasPack* pack = new CanvasPack;
	pack->_btn = _maker->GetTempBtnOneline();
	pack->_btn->GetText()->SetString(canvas_name);
	pack->_btn->SetMode(Control::RADIO);

	pack->_btnAddThis = _maker->GetTempBtnMenu();
	pack->_btnAddThis->GetText()->SetString(canvas_name);
	pack->_btnAddThis->GetCoor()->SetParent(_maker->GetCoorRight());
	pack->_btnAddThis->SetMode(Control::SWITCH);

	pack->_canvas = canvas;
	pack->_name = canvas_name;

	_mapCanvas.insert(make_pair(canvas_name, pack));
}

void SpriteMgr::AddSprite(String canvas_name, String spr_name, Sprite* spr)
{
	auto& iter = _mapCanvas.find(canvas_name);
	if (iter == _mapCanvas.end())
	{
		return;
	}

	SpriteNode node;
	node._btn = _maker->GetTempBtnOneline();
	node._btn->GetText()->SetString(spr_name);
	node._btn->GetText()->SetColor(0xffad2626);
	node._btn->SetMode(Control::RADIO);
	node._spr = spr;

	iter->second->_mapSprites[spr_name] = node;
}
