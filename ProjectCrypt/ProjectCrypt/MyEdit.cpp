#include "stdafx.h"
#include "MyEdit.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "Wall.h"
#include "SceneMgr.h"

CMyEdit::CMyEdit()
	:m_iDrawX(0), m_iDrawY(0), m_bObjectMode(false), m_bEraseMode(false), m_pPlayer(nullptr)
{
	ZeroMemory(m_szModeErase, sizeof(m_szModeErase));
	ZeroMemory(m_szModeObj, sizeof(m_szModeObj));
}

CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize()
{
	TILE_MGR->Load_Tiles(L"../Maps/EditTile.dat");
	OBJ_MGR->Load_Object(L"../Maps/EditObj.dat");

	m_pPlayer = OBJ_MGR->Get_Player();
	m_pPlayer->Set_Info_Pos(600.f, 600.f);
	TILE_MGR->Push_Object(m_pPlayer->Get_Info().fX, m_pPlayer->Get_Info().fY, m_pPlayer);
	m_pPlayer->Reset_Direction();

	srand((unsigned int)time(nullptr));
	lstrcpy(m_szModeErase, L"그리기 모드");
	lstrcpy(m_szModeObj, L"타일 모드");
	lstrcpy(m_szCurBlock, L"NONE");
}

void CMyEdit::Update()
{
	Key_Input();

	TILE_MGR->Update();
	OBJ_MGR->Update();
	OBJ_MGR->Get_Player()->Set_MoveTime(true);
}

void CMyEdit::Late_Update()
{
	if (INPUT_MGR->Key_Down(VK_F1))
	{
		SCENE_MGR->Scene_Change(SC_LOBBY);
		return;
	}

	TILE_MGR->Late_Update();
	OBJ_MGR->Late_Update();
}

void CMyEdit::Render(HDC _hDC)
{
	int iScrollX = (int)SCROLL_MGR->Get_ScrollX();
	int iScrollY = (int)SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(L"Ground");

	BitBlt(_hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	TILE_MGR->Render(_hDC);
	OBJ_MGR->Render(_hDC);

	TCHAR text[32]{};

	TextOut(_hDC, 650, 450, m_szCurBlock, lstrlen(m_szCurBlock));
	TextOut(_hDC, 650, 500, m_szModeErase, lstrlen(m_szModeErase));
	TextOut(_hDC, 650, 550, m_szModeObj, lstrlen(m_szModeObj));
}

void CMyEdit::Release()
{
}

void CMyEdit::Key_Input()
{
	// 모드 변경
	if (INPUT_MGR->Key_Down('M'))		// 오브젝트 모드, 타일 모드 변환
	{
		m_bObjectMode = !m_bObjectMode;

		if (m_bObjectMode)
		{
			lstrcpy(m_szModeObj, L"오브젝트 모드");
			lstrcpy(m_szCurBlock, L"NONE");
		}

		else
		{
			lstrcpy(m_szModeObj, L"타일 모드");
			lstrcpy(m_szCurBlock, L"NONE");
		}
	}

	if (INPUT_MGR->Key_Down(VK_RBUTTON))	// 우클릭 : 지우개 모드 변경
	{
		m_bEraseMode = !m_bEraseMode;

		if (m_bEraseMode)
		{
			lstrcpy(m_szModeErase, L"지우개 모드");
			lstrcpy(m_szCurBlock, L"NONE");
		}

		else
		{
			lstrcpy(m_szModeErase, L"그리기 모드");
			lstrcpy(m_szCurBlock, L"NONE");
		}
	}

	// 타일과 오브젝트를 bool변수를 이용해서 분리
	if (INPUT_MGR->Key_Down('1'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"wall_dirt_crypt");
			OBJ_MGR->Set_ObjID(OBJ_WALL);
			OBJ_MGR->Set_WallID(WALL_BREAK);
			lstrcpy(m_szCurBlock, L"흙 벽");
		}
		else
		{
			TILE_MGR->Set_ImgKey(L"floor_dirt1");
			m_iDrawX = 0;
			m_iDrawY = 0;
			lstrcpy(m_szCurBlock, L"기본 타일");
		}
	}

	if (INPUT_MGR->Key_Down('2'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"wall_stone_crypt");
			OBJ_MGR->Set_ObjID(OBJ_WALL);
			OBJ_MGR->Set_WallID(WALL_BOOM_BREAK);
			lstrcpy(m_szCurBlock, L"돌멩이 박힌 벽");
		}
		else
		{
			TILE_MGR->Set_ImgKey(L"stairs");
			m_iDrawX = 0;
			m_iDrawY = 0;
			lstrcpy(m_szCurBlock, L"계단 타일");
		}
	}

	if (INPUT_MGR->Key_Down('3'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"wall_torch");
			OBJ_MGR->Set_ObjID(OBJ_TORCH);
			OBJ_MGR->Set_WallID(WALL_BREAK);
			lstrcpy(m_szCurBlock, L"횃불");
		}
		else
		{
			TILE_MGR->Set_ImgKey(L"stairs_locked_miniboss");
			m_iDrawX = 0;
			m_iDrawY = 0;
			lstrcpy(m_szCurBlock, L"미니보스 계단");
		}
	}

	if (INPUT_MGR->Key_Down('4'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"wall_shop_crypt");
			OBJ_MGR->Set_ObjID(OBJ_WALL);
			OBJ_MGR->Set_WallID(WALL_BOOM_BREAK);
			lstrcpy(m_szCurBlock, L"금 벽");
		}
		else
		{
			TILE_MGR->Set_ImgKey(L"TEMP_shop_floor");
			m_iDrawX = 0;
			m_iDrawY = 0;
			lstrcpy(m_szCurBlock, L"상점 타일");
		}
	}

	if (INPUT_MGR->Key_Down('5'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"door_floor");
			OBJ_MGR->Set_ObjID(OBJ_WALL);
			OBJ_MGR->Set_WallID(WALL_DOOR);
			lstrcpy(m_szCurBlock, L"문(앞)");
		}
		else
		{
			TILE_MGR->Set_ImgKey(L"boss_floor_A");
			m_iDrawX = 0;
			m_iDrawY = 0;
			lstrcpy(m_szCurBlock, L"보스 타일");
		}
	}

	if (INPUT_MGR->Key_Down('6'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"door_side");
			OBJ_MGR->Set_ObjID(OBJ_WALL);
			OBJ_MGR->Set_WallID(WALL_DOOR);
			lstrcpy(m_szCurBlock, L"문(옆)");
		}
	}

	if (INPUT_MGR->Key_Down('7'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"end_of_world");
			OBJ_MGR->Set_ObjID(OBJ_WALL);
			OBJ_MGR->Set_WallID(WALL_UNBREAK);
			lstrcpy(m_szCurBlock, L"외 벽");
		}
	}

	if (INPUT_MGR->Key_Down('8'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"boss_wall");
			OBJ_MGR->Set_ObjID(OBJ_WALL);
			OBJ_MGR->Set_WallID(WALL_UNBREAK);
			lstrcpy(m_szCurBlock, L"보스 외벽");
		}
	}

	if (INPUT_MGR->Key_Down('9'))
	{
		static int i = 0;

		if (i > 3) i = 0;

		switch (i)
		{
		case 0:
			OBJ_MGR->Set_ImgKey(L"chest");
			OBJ_MGR->Set_ObjID(OBJ_CHEST);
			lstrcpy(m_szCurBlock, L"노말 상자");
			break;

		case 1:
			OBJ_MGR->Set_ImgKey(L"chest_black");
			OBJ_MGR->Set_ObjID(OBJ_CHEST);
			lstrcpy(m_szCurBlock, L"검은 상자");
			break;

		case 2:
			OBJ_MGR->Set_ImgKey(L"chest_locked");
			OBJ_MGR->Set_ObjID(OBJ_CHEST);
			lstrcpy(m_szCurBlock, L"잠긴 상자");
			break;

		case 3:
			OBJ_MGR->Set_ImgKey(L"chest_white");
			OBJ_MGR->Set_ObjID(OBJ_CHEST);
			lstrcpy(m_szCurBlock, L"흰색 상자");
			break;
		}
		++i;
	}

	if (INPUT_MGR->Key_Down('Q'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"slime_green");
			OBJ_MGR->Set_ObjID(OBJ_MONSTER);
			OBJ_MGR->Set_MonsterID(GREEN_SLIME);
			lstrcpy(m_szCurBlock, L"초록 슬라임");
		}
	}

	if (INPUT_MGR->Key_Down('W'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"slime_ice");
			OBJ_MGR->Set_ObjID(OBJ_MONSTER);
			OBJ_MGR->Set_MonsterID(ICE_SLIME);
			lstrcpy(m_szCurBlock, L"파란 슬라임");
		}
	}

	if (INPUT_MGR->Key_Down('E'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"skeleton");
			OBJ_MGR->Set_ObjID(OBJ_MONSTER);
			OBJ_MGR->Set_MonsterID(SKELETON);
			lstrcpy(m_szCurBlock, L"스켈레톤");
		}
	}

	if (INPUT_MGR->Key_Down('R'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"bat");
			OBJ_MGR->Set_ObjID(OBJ_MONSTER);
			OBJ_MGR->Set_MonsterID(BAT);
			lstrcpy(m_szCurBlock, L"박쥐");
		}
	}

	if (INPUT_MGR->Key_Down('T'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"zombie");
			OBJ_MGR->Set_ObjID(OBJ_MONSTER);
			OBJ_MGR->Set_MonsterID(ZOMBIE);
			lstrcpy(m_szCurBlock, L"좀비");
		}
	}

	if (INPUT_MGR->Key_Down('Y'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"dragon_green");
			OBJ_MGR->Set_ObjID(OBJ_MONSTER);
			OBJ_MGR->Set_MonsterID(GREEN_DRAGON);
			lstrcpy(m_szCurBlock, L"그린 드래곤");
		}
	}

	if (INPUT_MGR->Key_Down('A'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"king_conga");
			OBJ_MGR->Set_ObjID(OBJ_MONSTER);
			OBJ_MGR->Set_MonsterID(BOSS);
			lstrcpy(m_szCurBlock, L"보스");
		}
	}

	if (INPUT_MGR->Key_Down('S'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"shopkeeper");
			OBJ_MGR->Set_ObjID(OBJ_NPC);
			lstrcpy(m_szCurBlock, L"상점 주인");
		}
	}

	if (INPUT_MGR->Key_Down('Z'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"armor_platemail");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_ARMOR);
			lstrcpy(m_szCurBlock, L"방어구");
		}
	}

	if (INPUT_MGR->Key_Down('X'))
	{
		static int i = 0;

		if (i > 3) i = 0;

		switch (i)
		{
		case 0:
			OBJ_MGR->Set_ImgKey(L"food_1");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_FOOD);
			lstrcpy(m_szCurBlock, L"사과");
			break;

		case 1:
			OBJ_MGR->Set_ImgKey(L"food_2");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_FOOD);
			lstrcpy(m_szCurBlock, L"치즈");
			break;

		case 2:
			OBJ_MGR->Set_ImgKey(L"food_3");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_FOOD);
			lstrcpy(m_szCurBlock, L"닭고기");
			break;

		case 3:
			OBJ_MGR->Set_ImgKey(L"food_4");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_FOOD);
			lstrcpy(m_szCurBlock, L"큰 고기");
			break;
		}
		++i;
	}

	if (INPUT_MGR->Key_Down('C'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_ImgKey(L"bomb");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_BOMB);
			lstrcpy(m_szCurBlock, L"폭탄");
		}
	}

	if (INPUT_MGR->Key_Down('V'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_Obj_Money(0);
			OBJ_MGR->Set_ImgKey(L"torch_2");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_TORCH);
			lstrcpy(m_szCurBlock, L"횃불");
		}
	}

	if (INPUT_MGR->Key_Down('B'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_Obj_Money(0);
			OBJ_MGR->Set_ImgKey(L"misc_key");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_KEY);
			lstrcpy(m_szCurBlock, L"열쇠");
		}
	}

	if (INPUT_MGR->Key_Down('N'))
	{
		static int i = 0;

		if (i > 1) i = 0;

		if (i == 0)
		{
			OBJ_MGR->Set_ImgKey(L"shovel_basic");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_SHOVEL);
			lstrcpy(m_szCurBlock, L"삽");
		}
		else if (i == 1)
		{
			OBJ_MGR->Set_Obj_Money(0);
			OBJ_MGR->Set_ImgKey(L"pickaxe");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_SHOVEL);
			lstrcpy(m_szCurBlock, L"곡괭이");
		}
		++i;
	}

	if (INPUT_MGR->Key_Down('H'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_Obj_Money(0);
			OBJ_MGR->Set_ImgKey(L"scroll_purple");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_SPELL);
			lstrcpy(m_szCurBlock, L"스크롤");
		}
	}

	if (INPUT_MGR->Key_Down('J'))
	{
		if (m_bObjectMode)
		{
			OBJ_MGR->Set_Obj_Dia(0);
			OBJ_MGR->Set_ImgKey(L"misc_heart_container");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_HEART);
			lstrcpy(m_szCurBlock, L"최생 하트");
		}
	}

	if (INPUT_MGR->Key_Down('K'))
	{
		static int i = 0;

		if (i > 2) i = 0;

		switch (i)
		{
		case 0:
			OBJ_MGR->Set_ImgKey(L"weapon_dagger");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_WEAPON);
			lstrcpy(m_szCurBlock, L"단검");
			break;

		case 1:
			OBJ_MGR->Set_Obj_Money(0);
			OBJ_MGR->Set_ImgKey(L"weapon_broadsword_titanium");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_WEAPON);
			lstrcpy(m_szCurBlock, L"대검");
			break;

		case 2:
			OBJ_MGR->Set_ImgKey(L"weapon_longsword_golden");
			OBJ_MGR->Set_ObjID(OBJ_ITEM);
			OBJ_MGR->Set_ItemID(ITEM_WEAPON);
			lstrcpy(m_szCurBlock, L"장검");
			break;
		}
		++i;
	}
	// 피킹
	if (INPUT_MGR->Key_Pressing(VK_LBUTTON))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= long(SCROLL_MGR->Get_ScrollX());
		pt.y -= long(SCROLL_MGR->Get_ScrollY());

		if (m_bEraseMode)
		{
			if (m_bObjectMode)
				OBJ_MGR->Delete_Object(float(pt.x), float(pt.y));

			else
			{
				TILE_MGR->Set_ImgKey(L"floor_erase");
				TILE_MGR->Picking(pt, m_iDrawX, m_iDrawY);
			}
		}

		else
		{
			if (m_bObjectMode)
				OBJ_MGR->Picking(pt); //오브젝트 추가 함수
			else
				TILE_MGR->Picking(pt, m_iDrawX, m_iDrawY);
		}
	}

	if (INPUT_MGR->Key_Down('O'))	// 세이브
	{
		/*TILE_MGR->Save_Tiles(L"../Maps/StageOneTile.dat");
		OBJ_MGR->Save_Object(L"../Maps/StageOneObj.dat");*/

		/*TILE_MGR->Save_Tiles(L"../Maps/LobbyTile.dat");
		OBJ_MGR->Save_Object(L"../Maps/LobbyObj.dat");*/

		TILE_MGR->Save_Tiles(L"../Maps/EditTile.dat");
		OBJ_MGR->Save_Object(L"../Maps/EditObj.dat");

		/*TILE_MGR->Save_Tiles(L"../Maps/BossStageTile.dat");
		OBJ_MGR->Save_Object(L"../Maps/BossStageObj.dat");*/

		/*TILE_MGR->Save_Tiles(L"../Maps/BossStageTileStart.dat");
		OBJ_MGR->Save_Object(L"../Maps/BossStageObjStart.dat");*/

		/*TILE_MGR->Save_Tiles(L"../Maps/BossStageTileEnd.dat");
		OBJ_MGR->Save_Object(L"../Maps/BossStageObjEnd.dat");*/
	}

	if (INPUT_MGR->Key_Down('P'))	// 로드
	{
		/*TILE_MGR->Load_Tiles(L"../Maps/StageOneTile.dat");
		OBJ_MGR->Load_Object(L"../Maps/StageOneObj.dat");*/

		/*TILE_MGR->Load_Tiles(L"../Maps/LobbyTile.dat");
		OBJ_MGR->Load_Object(L"../Maps/LobbyObj.dat");*/

		TILE_MGR->Load_Tiles(L"../Maps/EditTile.dat");
		OBJ_MGR->Load_Object(L"../Maps/EditObj.dat");

		/*TILE_MGR->Load_Tiles(L"../Maps/BossStageTile.dat");
		OBJ_MGR->Load_Object(L"../Maps/BossStageObj.dat");*/

		/*TILE_MGR->Load_Tiles(L"../Maps/BossStageTileStart.dat");
		OBJ_MGR->Load_Object(L"../Maps/BossStageObjStart.dat");*/

		/*TILE_MGR->Load_Tiles(L"../Maps/BossStageTileEnd.dat");
		OBJ_MGR->Load_Object(L"../Maps/BossStageObjEnd.dat");*/
	}
}
