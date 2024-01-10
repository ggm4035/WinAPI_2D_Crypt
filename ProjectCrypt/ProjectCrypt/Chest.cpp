#include "stdafx.h"
#include "Chest.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "Weapon.h"
#include "Armor.h"
#include "Bomb.h"
#include "Food.h"
#include "Shovel.h"
#include "Spell.h"

CChest::CChest()
{
}

CChest::~CChest()
{
	Release();
}

bool CChest::Collision()
{
	return false;
}

void CChest::Make_Item()
{
	if (!lstrcmp(m_szImgKey, L"chest"))		// 음식 아이템 생성
	{
		int id = rand() % 4 + 1;
		CObj* pObj = new CFood; 
		
		switch (id)
		{
		case 1:
			pObj->Set_ImgKey(L"food_1");
			break;

		case 2:
			pObj->Set_ImgKey(L"food_2");
			break;

		case 3:
			pObj->Set_ImgKey(L"food_3");
			break;

		case 4:
			pObj->Set_ImgKey(L"food_4");
			break;
		}
		pObj->Set_Info_Pos(m_tInfo.fX, m_tInfo.fY);
		pObj->Initialize();
		TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, pObj);
		OBJ_MGR->AddObject(OBJ_ITEM, pObj);
	}
	else if (!lstrcmp(m_szImgKey, L"chest_black"))	// 폭탄 아이템 생성
	{
		CObj* pObj = new CBomb;
		pObj->Set_ImgKey(L"bomb");
		pObj->Set_Info_Pos(m_tInfo.fX, m_tInfo.fY);
		pObj->Initialize();
		TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, pObj);
		OBJ_MGR->AddObject(OBJ_ITEM, pObj);
	}
	else if (!lstrcmp(m_szImgKey, L"chest_white"))	// 스킬 오브젝트 생성
	{
		CObj* pObj = new CSpell;
		pObj->Set_ImgKey(L"scroll_purple");
		pObj->Set_Info_Pos(m_tInfo.fX, m_tInfo.fY);
		pObj->Initialize();
		TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, pObj);
		OBJ_MGR->AddObject(OBJ_ITEM, pObj);
	}
	else if (!lstrcmp(m_szImgKey, L"chest_locked"))	// 방어구 생성
	{
		int id = rand() % 5 + 1;

		CObj* pObj = new CArmor;
		pObj->Set_Info_Pos(m_tInfo.fX, m_tInfo.fY);
		switch (id)
		{
		case 1:
			pObj->Set_ImgKey(L"armor_leather");
			break;
		case 2:
			pObj->Set_ImgKey(L"armor_chainmail");
			break;
		case 3:
			pObj->Set_ImgKey(L"armor_platemail");
			break;
		case 4:
			pObj->Set_ImgKey(L"armor_heavyplate");
			break;
		case 5:
			pObj->Set_ImgKey(L"armor_gi");
			break;
		}
		static_cast<CArmor*>(pObj)->Set_Armor(Armor(id));
		pObj->Initialize();

		TILE_MGR->Push_Object(m_tInfo.fX, m_tInfo.fY, pObj);
		OBJ_MGR->AddObject(OBJ_ITEM, pObj);
	}
}

void CChest::Initialize()
{
	m_eID = OBJ_CHEST;
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
	m_tState.iHP = 1;
}

int CChest::Update()
{
	if (m_bDead)
		return OBJ_DEAD;	// 아이템 드랍 시점

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CChest::Late_Update()
{
	if (m_tState.iHP <= 0)
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		Make_Item();
		m_bDead = true;
	}
}

void CChest::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 10),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, int(m_tInfo.fCY * m_tFrame.iMotion),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}

void CChest::Release()
{
}
