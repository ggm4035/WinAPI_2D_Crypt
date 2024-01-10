#include "stdafx.h"
#include "ObjMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "Player.h"
#include "Wall.h"
#include "Torch.h"
#include "GreenSlime.h"
#include "IceSlime.h"
#include "ScrollMgr.h"
#include "Skeleton.h"
#include "Bat.h"
#include "Boss.h"
#include "Dragon.h"
#include "ShopKeeper.h"
#include "Weapon.h"
#include "Shovel.h"
#include "Food.h"
#include "Bomb.h"
#include "Armor.h"
#include "Chest.h"
#include "Key.h"
#include "ItemTorch.h"
#include "SceneMgr.h"
#include "Spell.h"
#include "Zombie.h"
#include "Dagger.h"
#include "BroadSword.h"
#include "LongSword.h"
#include "Heart.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
	:m_pImgKey(nullptr), m_eItemID(ITEM_END), m_iMoney(0), m_bQuake(false), m_eMonID(MONSTER_END),
	m_eObjID(OBJ_END), m_eWallID(WALL_END), m_iDia(0)
{
}

CObjMgr::~CObjMgr()
{
	Release();
	Release_Player();
}

void CObjMgr::Make_Player()
{
	CObj* pPlayer = new CPlayer;
	pPlayer->Set_Info_Pos(600, 600);
	pPlayer->Initialize();
	TILE_MGR->Push_Object(pPlayer->Get_Info().fX, pPlayer->Get_Info().fY, pPlayer);
	AddObject(OBJ_PLAYER, pPlayer);
}

void CObjMgr::AddObject(OBJID _eID, CObj * _pObj)
{
	m_ObjList[_eID].push_back(_pObj);
}

void CObjMgr::Initialize()
{
	Read_Resource();
}

void CObjMgr::Update()
{
	int iCullX = int(abs(SCROLL_MGR->Get_ScrollX()));
	int iCullY = int(abs(SCROLL_MGR->Get_ScrollY()));

	if (SCROLL_MGR->Get_ScrollX() / (TILECX) > 0) iCullX = 0;
	if (SCROLL_MGR->Get_ScrollY() / (TILECY) > 0) iCullY = 0;

	int iMaxX = iCullX + WINCX;
	int iMaxY = iCullY + WINCY;

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		int iResult = 0;
		for (auto& iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); )
		{
			if ((*iter)->Get_ObjID() == OBJ_PLAYER || (*iter)->Get_ObjID() == OBJ_NPC || SCENE_MGR->Get_Cur_Scene() == SC_BOSS_STAGE ||
				((*iter)->Get_Info().fX > iCullX && (*iter)->Get_Info().fX < iMaxX + TILECX &&
					(*iter)->Get_Info().fY > iCullY && (*iter)->Get_Info().fY < iMaxY + TILECY))
			{
				iResult = (*iter)->Update();

				if (iResult == PLAYER_DEAD)
					(*iter)->Set_HP_Full();

				if (iResult == OBJ_DEAD)
				{
					SAFE_DELETE(CObj*)(*iter);
					iter = m_ObjList[i].erase(iter);
				}
				else
					++iter;
			}
			else
				++iter;
		}
	}
}

void CObjMgr::Late_Update()
{
	int iCullX = int(abs(SCROLL_MGR->Get_ScrollX()));
	int iCullY = int(abs(SCROLL_MGR->Get_ScrollY()));

	if (SCROLL_MGR->Get_ScrollX() / (TILECX) > 0) iCullX = 0;
	if (SCROLL_MGR->Get_ScrollY() / (TILECY) > 0) iCullY = 0;

	int iMaxX = iCullX + WINCX;
	int iMaxY = iCullY + WINCY;

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
		{
			if (iter->Get_ObjID() == OBJ_PLAYER || iter->Get_ObjID() == OBJ_NPC || SCENE_MGR->Get_Cur_Scene() == SC_BOSS_STAGE ||
				(iter->Get_Info().fX > iCullX && iter->Get_Info().fX < iMaxX + TILECX &&
					iter->Get_Info().fY > iCullY && iter->Get_Info().fY < iMaxY + TILECY))
			{
				iter->Late_Update();
				m_RenderList.push_back(iter);
			}
		}
	}
}

void CObjMgr::Render(HDC _hDC)
{
	if (m_bQuake)
	{
		SCROLL_MGR->Quake();
		m_bQuake = false;
	}

	int iCullX = int(abs(SCROLL_MGR->Get_ScrollX()));
	int iCullY = int(abs(SCROLL_MGR->Get_ScrollY()));

	if (SCROLL_MGR->Get_ScrollX() / (TILECX) > 0) iCullX = 0;
	if (SCROLL_MGR->Get_ScrollY() / (TILECY) > 0) iCullY = 0;

	int iMaxX = iCullX + WINCX;
	int iMaxY = iCullY + WINCY;

	m_RenderList.sort([](CObj* _pDest, CObj* _pSrc)->bool
	{
		return _pDest->Get_Info().fY < _pSrc->Get_Info().fY;
	});

	for (auto& iter : m_RenderList)
	{
		if (iter->Get_Info().fX > iCullX && iter->Get_Info().fX < iMaxX + TILECX &&
			iter->Get_Info().fY > iCullY && iter->Get_Info().fY < iMaxY + TILECY)
			iter->Render(_hDC);
	}

	m_RenderList.clear();
}

void CObjMgr::Release()
{
	for (size_t i = OBJ_WALL; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), SAFE_DELETE(CObj*));
		m_ObjList[i].clear();
	}
}

void CObjMgr::Release_Player()
{
	if (!m_ObjList[OBJ_PLAYER].empty())
		SAFE_DELETE(CObj*)(m_ObjList[OBJ_PLAYER].front());
}

void CObjMgr::Picking(const POINT & _pt)
{
	int iX = _pt.x / int(TILECX);
	int iY = _pt.y / int(TILECY);

	int idx = iY * TILEX + iX;


	if (iX < 0 || iY < 0 || iX * iY > TILEX * TILEY)
		return;

	float fX = (TILECX *  0.5f) + float(iX * TILECX);
	float fY = (TILECY *  0.5f) + float(iY * TILECY);

	CObj* pObj = nullptr;

	switch (m_eObjID)
	{
	case OBJ_PLAYER:
		break;

	case OBJ_WALL:
		if (Find_Obj(fX, fY))
			return;

		pObj = new CWall;
		pObj->Set_ImgKey(m_pImgKey);
		pObj->Set_Info_Pos(fX, fY);
		static_cast<CWall*>(pObj)->Set_WallID(m_eWallID);
		pObj->Initialize();
		TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
		AddObject(OBJ_WALL, pObj);
		break;

	case OBJ_TORCH:
		if (Find_Obj(OBJ_TORCH, fX, fY))
			return;

		pObj = new CTorch;
		pObj->Set_ImgKey(m_pImgKey);
		pObj->Set_Info_Pos(fX, fY);
		pObj->Initialize();
		TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
		AddObject(OBJ_TORCH, pObj);
		break;

	case OBJ_CHEST:
		if (Find_Obj(fX, fY))
			return;

		pObj = new CChest;
		pObj->Set_ImgKey(m_pImgKey);
		pObj->Set_Info_Pos(fX, fY);
		pObj->Initialize();
		TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
		AddObject(OBJ_CHEST, pObj);
		break;

	case OBJ_MONSTER:
		if (Find_Obj(fX, fY))
			return;

		switch (m_eMonID)
		{
		case GREEN_SLIME:
			pObj = new CGreenSlime;
			break;

		case ICE_SLIME:
			pObj = new CIceSlime;
			break;

		case SKELETON:
			pObj = new CSkeleton;
			break;

		case BAT:
			pObj = new CBat;
			break;

		case ZOMBIE:
			pObj = new CZombie;
			break;

		case GREEN_DRAGON:
			pObj = new CDragon;
			break;

		case BOSS:
			pObj = new CBoss;
			break;
		}
		pObj->Set_ImgKey(m_pImgKey);
		pObj->Set_Info_Pos(fX, fY);
		pObj->Initialize();
		TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
		AddObject(OBJ_MONSTER, pObj);
		break;

	case OBJ_NPC:
		if (Find_Obj(fX, fY))
			return;

		pObj = new CShopKeeper;
		pObj->Set_ImgKey(m_pImgKey);
		pObj->Set_Info_Pos(fX, fY);
		pObj->Initialize();
		TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
		AddObject(OBJ_NPC, pObj);
		break;

	case OBJ_ITEM:
		if (Find_Obj(fX, fY) && Find_Obj(fX, fY)->Get_ObjID() == OBJ_ITEM)
			return;

		switch (m_eItemID)
		{
		case ITEM_SHOVEL:
			pObj = new CShovel;
			break;

		case ITEM_WEAPON:
			if (!lstrcmp(m_pImgKey, L"weapon_dagger"))
				pObj = new CDagger;

			if (!lstrcmp(m_pImgKey, L"weapon_broadsword_titanium"))
				pObj = new CBroadSword;

			if (!lstrcmp(m_pImgKey, L"weapon_longsword_golden"))
				pObj = new CLongSword;
			break;

		case ITEM_ARMOR:
			pObj = new CArmor;
			break;

		case ITEM_TORCH:
			pObj = new CItemTorch;
			break;

		case ITEM_BOMB:
			pObj = new CBomb;
			break;

		case ITEM_FOOD:
			pObj = new CFood;
			break;

		case ITEM_KEY:
			pObj = new CKey;
			break;

		case ITEM_SPELL:
			pObj = new CSpell;
			break;

		case ITEM_HEART:
			pObj = new CHeart;
			break;
		}
		pObj->Set_ImgKey(m_pImgKey);
		pObj->Set_Info_Pos(fX, fY);
		pObj->Set_Money(m_iMoney);
		pObj->Set_Dia(m_iDia);
		pObj->Initialize();
		TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
		AddObject(OBJ_ITEM, pObj);
		break;
	}
}

void CObjMgr::Delete_Object(const float& _fX, const float& _fY)
{
	CObj* pObj = TILE_MGR->Pop_Object(_fX, _fY);

	if (!pObj || pObj->Get_ObjID() == OBJ_PLAYER)
		return;

	Erase_Object(pObj);

	SAFE_DELETE(CObj*)(pObj);
}

void CObjMgr::Erase_Object(const CObj * _pObj)
{
	if (!_pObj)
		return;

	auto& iter = find_if(m_ObjList[_pObj->Get_ObjID()].begin(), m_ObjList[_pObj->Get_ObjID()].end(), [&](CObj* _Dest)
	{
		if ((_Dest->Get_Info().fX == _pObj->Get_Info().fX) && (_Dest->Get_Info().fY == _pObj->Get_Info().fY))
			return true;

		else
			return false;
	});

	if (iter != m_ObjList[_pObj->Get_ObjID()].end())
		m_ObjList[_pObj->Get_ObjID()].erase(iter);
}

void CObjMgr::Set_Hitting_Obj(OBJID _eID)
{
	for (auto& iter : m_ObjList[_eID])
		iter->Set_MoveTime(true);
}

void CObjMgr::Set_Stop_Monster(bool _bool)
{
	for (auto& iter : m_ObjList[OBJ_MONSTER])
		static_cast<CMonster*>(iter)->Set_Stop_Monster(_bool);
}

CObj * CObjMgr::Find_Obj(const OBJID& _eID, const float _fX, const float _fY)
{
	auto& iter = find_if(m_ObjList[_eID].begin(), m_ObjList[_eID].end(), [&](CObj* _Dest)
	{
		if (((_Dest->Get_Next_Info().fX / TILECX) == (_fX / TILECX)) &&
			((_Dest->Get_Next_Info().fY / TILECY) == (_fY / TILECY)))
			return true;

		else
			return false;
	});

	if (iter != m_ObjList[_eID].end())
		return *iter;

	return nullptr;
}

CObj * CObjMgr::Find_Obj(const float _fX, const float _fY)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		auto& iter = find_if(m_ObjList[i].begin(), m_ObjList[i].end(), [&](CObj* _Dest)
		{
			if ((_Dest->Get_Info().fX == _fX) && (_Dest->Get_Info().fY == _fY))
				return true;

			else
				return false;
		});

		if (iter != m_ObjList[i].end())
			return *iter;
	}

	return nullptr;
}

CObj * CObjMgr::Find_Monster(MONSTERID _eMonID)
{
	auto& iter = find_if(m_ObjList[OBJ_MONSTER].begin(), m_ObjList[OBJ_MONSTER].end(), [&](CObj* _Dest)
	{
		if (static_cast<CMonster*>(_Dest)->Get_MonsterID() == _eMonID)
			return true;

		else
			return false;
	});
	if (iter != m_ObjList[OBJ_MONSTER].end())
		return *iter;

	else
		return nullptr;
}

void CObjMgr::Save_Object(const TCHAR * _pFilePath)
{
	HANDLE hFile = CreateFile(_pFilePath,		// 파일 경로
		GENERIC_WRITE,							// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,									// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
		NULL,									// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
		CREATE_ALWAYS,							// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
		FILE_ATTRIBUTE_NORMAL,					// 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
		NULL);									// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		TCHAR error[32];
		swprintf_s(error, L"error code : %d", GetLastError());
		MessageBox(g_hWnd, _T("Fail(Object)"), error, MB_OK);
		return;
	}

	DWORD dwByte = 0;
	TCHAR szImgKey[32]{};

	for (size_t i = OBJ_WALL; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
		{
			WriteFile(hFile, &iter->Get_ObjID(), sizeof(OBJID), &dwByte, nullptr);
			WriteFile(hFile, &iter->Get_Info(), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &iter->Get_State(), sizeof(STATE), &dwByte, nullptr);
			WriteFile(hFile, iter->Get_ImgKey(), sizeof(TCHAR) * 32, &dwByte, nullptr);

			if (i == OBJ_WALL)
				WriteFile(hFile, &static_cast<CWall*>(iter)->Get_WallID(), sizeof(WALLID), &dwByte, nullptr);

			else if (i == OBJ_MONSTER)
				WriteFile(hFile, &static_cast<CMonster*>(iter)->Get_MonsterID(), sizeof(MONSTERID), &dwByte, nullptr);

			else if (i == OBJ_ITEM)
				WriteFile(hFile, &static_cast<CItem*>(iter)->Get_ItemID(), sizeof(ITEMID), &dwByte, nullptr);
		}
	}

	CloseHandle(hFile);
}

void CObjMgr::Load_Object(const TCHAR * _pFilePath)
{
	Release();	// 플레이어는 삭제하면 안됨 비울건 벽, 몬스터, 아이템만 비우게 만들기 (아직 안건듦)

	HANDLE hFile = CreateFile(_pFilePath,		// 파일 경로
		GENERIC_READ,							// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,									// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
		NULL,									// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
		OPEN_EXISTING,							// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
		FILE_ATTRIBUTE_NORMAL,					// 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
		NULL);									// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL)

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, _T("Fail(Object)"), L"Load File", MB_OK);
		return;
	}

	DWORD dwByte = 0;

	OBJID eID = OBJ_END;
	WALLID eWallID = WALL_END;
	ITEMID eItemID = ITEM_END;
	MONSTERID eMonID = MONSTER_END;

	INFO tInfo{};
	STATE tState{};
	TCHAR szImgKey[32]{};

	while (true)
	{
		ReadFile(hFile, &eID, sizeof(OBJID), &dwByte, nullptr);
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &tState, sizeof(STATE), &dwByte, nullptr);
		ReadFile(hFile, szImgKey, sizeof(szImgKey), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		CObj* pObj = nullptr;

		switch (eID)
		{
		case OBJ_WALL:
			ReadFile(hFile, &eWallID, sizeof(WALLID), &dwByte, nullptr);

			pObj = new CWall;
			static_cast<CWall*>(pObj)->Set_WallID(eWallID);
			pObj->Set_Info_Pos(tInfo.fX, tInfo.fY);
			pObj->Set_ImgKey(szImgKey);
			pObj->Initialize();
			TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
			AddObject(OBJ_WALL, pObj);
			break;

		case OBJ_TORCH:
			pObj = new CTorch;
			pObj->Set_Info_Pos(tInfo.fX, tInfo.fY);
			pObj->Set_ImgKey(szImgKey);
			pObj->Initialize();
			TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
			AddObject(OBJ_TORCH, pObj);
			break;

		case OBJ_CHEST:
			pObj = new CChest;
			pObj->Set_Info_Pos(tInfo.fX, tInfo.fY);
			pObj->Set_ImgKey(szImgKey);
			pObj->Initialize();
			TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
			AddObject(OBJ_CHEST, pObj);
			break;

		case OBJ_MONSTER:
			ReadFile(hFile, &eMonID, sizeof(MONSTERID), &dwByte, nullptr);
			switch (eMonID)
			{
			case GREEN_SLIME:
				pObj = new CGreenSlime;
				break;

			case ICE_SLIME:
				pObj = new CIceSlime;
				break;

			case SKELETON:
				pObj = new CSkeleton;
				break;

			case BAT:
				pObj = new CBat;
				break;

			case ZOMBIE:
				pObj = new CZombie;
				break;

			case GREEN_DRAGON:
				pObj = new CDragon;
				break;

			case BOSS:
				pObj = new CBoss;
				break;
			}
			pObj->Set_Info_Pos(tInfo.fX, tInfo.fY);
			pObj->Set_ImgKey(szImgKey);
			pObj->Initialize();
			TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
			AddObject(OBJ_MONSTER, pObj);
			break;

		case OBJ_NPC:
			pObj = new CShopKeeper;
			pObj->Set_ImgKey(szImgKey);
			pObj->Set_Info_Pos(tInfo.fX, tInfo.fY);
			pObj->Initialize();
			TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
			AddObject(OBJ_NPC, pObj);
			break;

		case OBJ_ITEM:
			ReadFile(hFile, &eItemID, sizeof(ITEMID), &dwByte, nullptr);
			switch (eItemID)
			{
			case ITEM_SHOVEL:
				pObj = new CShovel;
				break;

			case ITEM_WEAPON:
				if (!lstrcmp(szImgKey, L"weapon_dagger"))
					pObj = new CDagger;

				if (!lstrcmp(szImgKey, L"weapon_broadsword_titanium"))
					pObj = new CBroadSword;

				if (!lstrcmp(szImgKey, L"weapon_longsword_golden"))
					pObj = new CLongSword;
				break;

			case ITEM_ARMOR:
				pObj = new CArmor;
				break;

			case ITEM_TORCH:
				pObj = new CItemTorch;
				break;

			case ITEM_BOMB:
				pObj = new CBomb;
				break;

			case ITEM_FOOD:
				pObj = new CFood;
				break;

			case ITEM_KEY:
				pObj = new CKey;
				break;

			case ITEM_SPELL:
				pObj = new CSpell;
				break;

			case ITEM_HEART:
				pObj = new CHeart;
				break;
			}
			pObj->Set_ImgKey(szImgKey);
			pObj->Set_Info_Pos(tInfo.fX, tInfo.fY);
			pObj->Set_Money(tState.iMoney);
			pObj->Set_Dia(tState.iDiamond);
			pObj->Initialize();
			TILE_MGR->Push_Object(pObj->Get_Info().fX, pObj->Get_Info().fY, pObj);
			AddObject(OBJ_ITEM, pObj);
			break;
		}
	}

	CloseHandle(hFile);
}

void CObjMgr::Read_Resource()
{
	// 플레이어
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body.bmp", L"player1_armor_body");					// 몸통 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_Flip.bmp", L"player1_armor_body_Flip");			// 몸통 왼쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_hit.bmp", L"player1_armor_body_hit");			// 히트 모션 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_hit_Flip.bmp", L"player1_armor_body_hit_Flip");	// 히트 모션 왼쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads.bmp", L"player1_heads");								// 머리 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_Flip.bmp", L"player1_heads_Flip");					// 머리 왼쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_hit.bmp", L"player1_heads_hit");						// 히트 머리 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_hit_Flip.bmp", L"player1_heads_hit_Flip");			// 히트 머리 왼쪽

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_img.bmp", L"player1_armor_body_img");			// 잔상 모션 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_img_Flip.bmp", L"player1_armor_body_img_Flip");	// 잔상 모션 왼쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_img.bmp", L"player1_heads_img");						// 잔상 머리 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_img_Flip.bmp", L"player1_heads_img_Flip");			// 잔상 머리 왼쪽

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_img1.bmp", L"player1_armor_body_img1");			// 잔상 모션 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_img1_Flip.bmp", L"player1_armor_body_img1_Flip");// 잔상 모션 왼쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_img1.bmp", L"player1_heads_img1");					// 잔상 머리 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_img1_Flip.bmp", L"player1_heads_img1_Flip");			// 잔상 머리 왼쪽

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_img2.bmp", L"player1_armor_body_img2");			// 잔상 모션 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_img2_Flip.bmp", L"player1_armor_body_img2_Flip");// 잔상 모션 왼쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_img2.bmp", L"player1_heads_img2");					// 잔상 머리 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_img2_Flip.bmp", L"player1_heads_img2_Flip");			// 잔상 머리 왼쪽

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_img3.bmp", L"player1_armor_body_img3");			// 잔상 모션 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_armor_body_img3_Flip.bmp", L"player1_armor_body_img3_Flip");// 잔상 모션 왼쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_img3.bmp", L"player1_heads_img3");					// 잔상 머리 오른쪽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/player1_heads_img3_Flip.bmp", L"player1_heads_img3_Flip");			// 잔상 머리 왼쪽

	// 몬스터
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/slime_green.bmp", L"slime_green");					// 초록 슬라임
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/slime_green_Flip.bmp", L"slime_green_Flip");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/slime_ice.bmp", L"slime_ice");						// 파란 슬라임
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/slime_ice_Flip.bmp", L"slime_ice_Flip");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/skeleton.bmp", L"skeleton");						//스켈레톤
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/skeleton_Flip.bmp", L"skeleton_Flip");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/bat.bmp", L"bat");									// 박쥐
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/bat_Flip.bmp", L"bat_Flip");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/zombie.bmp", L"zombie");							// 좀비
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/zombie_Flip.bmp", L"zombie_Flip");

	// 미니보스
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/bat_miniboss_yellow.bmp", L"bat_miniboss_yellow");				// 황금박쥐
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/bat_miniboss_yellow_Flip.bmp", L"bat_miniboss_yellow_Flip");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/dragon_green.bmp", L"dragon_green");							// 그린 드래곤
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/dragon_green_Flip.bmp", L"dragon_green_Flip");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/dragon_red.bmp", L"dragon_red");								// 레드 드래곤
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/dragon_red_Flip.bmp", L"dragon_red_Flip");

	// 보스
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/king_conga.bmp", L"king_conga");						// 킹 콩 가
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/king_conga_throne.bmp", L"king_conga_throne");			// 킹 콩 가 의 자

	// NPC
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/shopkeeper.bmp", L"shopkeeper");				// 상점 주인
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/dungeon_master.bmp", L"dungeon_master");		// 던전 마스터

	// 벽 
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/door_front.bmp", L"door_floor");								// 문(앞)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/door_side.bmp", L"door_side");									// 문(옆)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/wall_shop_crypt.bmp", L"wall_shop_crypt");						// 골드 벽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/wall_stone_crypt.bmp", L"wall_stone_crypt");					// 돌맹이 박힌 벽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/wall_torch.bmp", L"wall_torch");								// 횃불
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/wall_dirt_crypt.bmp", L"wall_dirt_crypt");						// 흙벽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/end_of_world.bmp", L"end_of_world");							// 외벽(못 부숨)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/boss_wall.bmp", L"boss_wall");									// 보스 외벽(못 부숨)
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/wall_catacomb_crypt1.bmp", L"wall_catacomb_crypt1");			// 카타콤1(해골 박혀있는 돌벽)
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/wall_catacomb_crypt2", L"wall_catacomb_crypt2");				// 카타콤2
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/wall_shop_crypt_cracked.bmp", L"wall_shop_crypt_cracked");	// 골드 벽(금간 벽)
	//BITMAP_MGR->Insert_Bmp(L"../Datas/Data/level/wall_dirt_crypt_diamond2.bmp", L"wall_dirt_crypt_diamond2");	// 다이아몬드 박힌 벽

	// 이펙트
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_dagger.bmp", L"swipe_dagger");										// 단검 이펙트
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_dagger_Down.bmp", L"swipe_dagger_Down");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_dagger_Left.bmp", L"swipe_dagger_Left");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_dagger_Up.bmp", L"swipe_dagger_Up");

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_broadsword_titanium.bmp", L"swipe_broadsword_titanium");			// 대검 이펙트
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_broadsword_titanium_Down.bmp", L"swipe_broadsword_titanium_Down");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_broadsword_titanium_Left.bmp", L"swipe_broadsword_titanium_Left");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_broadsword_titanium_Up.bmp", L"swipe_broadsword_titanium_Up");

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_longsword_gold.bmp", L"swipe_longsword_gold");						// 장검 이펙트
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_longsword_gold_Down.bmp", L"swipe_longsword_gold_Down");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_longsword_gold_Left.bmp", L"swipe_longsword_gold_Left");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_longsword_gold_Up.bmp", L"swipe_longsword_gold_Up");

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/swipes/swipe_enemy_Left.bmp", L"swipe_enemy_Left");								// 몬스터 공격 이펙트

	// UI(플레이어)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/heart.bmp", L"heart");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/heart_empty.bmp", L"heart_empty");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/heart_half.bmp", L"heart_half");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/hud_coins.bmp", L"hud_coins");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/diamond.bmp", L"diamond");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/hud_slot_1.bmp", L"hud_slot_1");				// 삽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/hud_slot_2.bmp", L"hud_slot_2");				// 무기
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/hud_slot_3.bmp", L"hud_slot_3");				// 갑옷
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/hud_slot_6.bmp", L"hud_slot_6");				// 횃불
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/hud_slot_8.bmp", L"hud_slot_8");				// 열쇠
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/hud_slot_bomb.bmp", L"hud_slot_bomb");		// 폭탄
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/hud_slot_action1.bmp", L"hud_slot_action1");	// 아이템(체력 회복)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/hud_slot_spell1.bmp", L"hud_slot_spell1");	// 스펠

	// UI(몬스터)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/TEMP_heart_small.bmp", L"TEMP_heart_small");
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/gui/TEMP_heart_empty_small.bmp", L"TEMP_heart_empty_small");

	// 아이템
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/shovel_basic.bmp", L"shovel_basic");			// 삽
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/pickaxe.bmp", L"pickaxe");						// 곡괭이

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/weapon_dagger.bmp", L"weapon_dagger");			// 무기
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/weapon_broadsword_titanium.bmp", L"weapon_broadsword_titanium");	// 무기(대검)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/weapon_longsword_golden.bmp", L"weapon_longsword_golden");			// 무기(장검)

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/armor_leather.bmp", L"armor_leather");			// 갑옷(가죽)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/armor_chainmail.bmp", L"armor_chainmail");		// 갑옷(체인)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/armor_platemail.bmp", L"armor_platemail");		// 갑옷
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/armor_heavyplate.bmp", L"armor_heavyplate");	// 갑옷(헤비)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/armor_gi.bmp", L"armor_gi");					// 갑옷(도복)

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/torch_2.bmp", L"torch_2");						// 횃불

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/bomb.bmp", L"bomb");							// 폭탄
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/3x3_explosion.bmp", L"3x3_explosion");			// 폭탄 터짐

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/food_1.bmp", L"food_1");						// 음식(사과)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/food_2.bmp", L"food_2");						// 음식(치즈)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/food_3.bmp", L"food_3");						// 음식(닭다리)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/food_4.bmp", L"food_4");						// 음식(큰 고기)

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/misc_key.bmp", L"misc_key");					// 열쇠
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/scroll_purple.bmp", L"scroll_purple");			// 스크롤

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/gold_small.bmp", L"gold_small");				// 골드
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/resource_diamond.bmp", L"resource_diamond");	// 다이아몬드

	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/items/misc_heart_container.bmp", L"misc_heart_container");	// 최대체력 하트

	// 상자
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/chest.bmp", L"chest");							// 상자1(노말)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/chest_black.bmp", L"chest_black");				// 상자2(블랙)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/chest_locked.bmp", L"chest_locked");			// 상자3(잠김)
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/entities/chest_white.bmp", L"chest_white");				// 상자4(흰색)

	// 함정
	BITMAP_MGR->Insert_Bmp(L"../Datas/Data/traps/bouncetrap.bmp", L"bouncetrap");					// 바운스 함정
}
