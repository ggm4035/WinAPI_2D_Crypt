#include "stdafx.h"
#include "Bomb.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"
#include "ObjMgr.h"

CBomb::CBomb()
	:m_iCount(0), m_blit(false), m_iDrawX(0), m_bBoom(false)
{
}

CBomb::~CBomb()
{
	Release();
}

bool CBomb::Collision()
{
	return false;
}

void CBomb::Initialize()
{
	m_eID = OBJ_ITEM;
	m_eItemID = ITEM_BOMB;
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
	m_iCount = 1;
	lstrcpy(m_szImgKey, L"bomb");
}

int CBomb::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBomb::Late_Update()
{
	if (m_iCount <= 0)
	{
		__super::Move_Frame(m_tFrame);

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
			m_bDead = true;
		}

		return;
	}

	if (m_blit)
	{
		if (m_bMoveTime)
		{
			m_tFrame.iFrameStart = 2 + m_iDrawX;
			++m_iDrawX;

			if (m_iDrawX > 3)
				m_bBoom = true;

			m_bMoveTime = false;
		}
	}

	if (m_bBoom)
	{
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 7;
		m_tFrame.dwSpeed = 100;
		m_tFrame.dwTime = GetTickCount();
		SOUND_MGR->PlaySound(L"sfx_bomb_explode.ogg", SOUND_EFFECT, 0.3f);
		Set_Damages();
		--m_iCount;
	}
}

void CBomb::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	if (m_bBoom)
	{
		HDC hMemDC = BITMAP_MGR->Find_Img(L"3x3_explosion"); 

		GdiTransparentBlt(_hDC, int(m_tInfo.fX - 74.f + fScrollX), int(m_tInfo.fY - 74.f + fScrollY),
			148, 148, hMemDC,
			int(148 * m_tFrame.iFrameStart), 0,
			148, 148, RGB(255, 0, 144));
	}
	else
	{
		HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

		GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 20),
			int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
			int((m_tInfo.fCX) * m_tFrame.iFrameStart), int((m_tInfo.fCY) * m_tFrame.iMotion),
			int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
	}
	__super::Show_Money(_hDC);
}

void CBomb::Release()
{
	m_iCount = 0;
}

void CBomb::Render_UI(HDC _hDC)
{
	if (m_iCount == 0)
		return;

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC,
		int(m_tInfo.fX - m_tInfo.fCX),
		int(m_tInfo.fY - m_tInfo.fCY),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, 0,
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}

void CBomb::Render_Text(HDC _hDC)
{
}

void CBomb::Set_Damages()
{
	CObj* pObj = OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tInfo.fX - TILECX, m_tInfo.fY);
	if(pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tInfo.fX + TILECX, m_tInfo.fY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tInfo.fX - TILECX, m_tInfo.fY - TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tInfo.fX + TILECX, m_tInfo.fY - TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tInfo.fX - TILECX, m_tInfo.fY + TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tInfo.fX + TILECX, m_tInfo.fY + TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tInfo.fX, m_tInfo.fY - TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_MONSTER, m_tInfo.fX, m_tInfo.fY + TILECY);
	if (pObj)
		pObj->Set_Damage(3);

	pObj = OBJ_MGR->Find_Obj(OBJ_PLAYER, m_tInfo.fX - TILECX, m_tInfo.fY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_PLAYER, m_tInfo.fX + TILECX, m_tInfo.fY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_PLAYER, m_tInfo.fX - TILECX, m_tInfo.fY - TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_PLAYER, m_tInfo.fX + TILECX, m_tInfo.fY - TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_PLAYER, m_tInfo.fX - TILECX, m_tInfo.fY + TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_PLAYER, m_tInfo.fX + TILECX, m_tInfo.fY + TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_PLAYER, m_tInfo.fX, m_tInfo.fY - TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_PLAYER, m_tInfo.fX, m_tInfo.fY + TILECY);
	if (pObj)
		pObj->Set_Damage(3);

	pObj = OBJ_MGR->Find_Obj(OBJ_WALL, m_tInfo.fX - TILECX, m_tInfo.fY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_WALL, m_tInfo.fX + TILECX, m_tInfo.fY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_WALL, m_tInfo.fX - TILECX, m_tInfo.fY - TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_WALL, m_tInfo.fX + TILECX, m_tInfo.fY - TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_WALL, m_tInfo.fX - TILECX, m_tInfo.fY + TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_WALL, m_tInfo.fX + TILECX, m_tInfo.fY + TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_WALL, m_tInfo.fX, m_tInfo.fY - TILECY);
	if (pObj)
		pObj->Set_Damage(3);
	pObj = OBJ_MGR->Find_Obj(OBJ_WALL, m_tInfo.fX, m_tInfo.fY + TILECY);
	if (pObj)
		pObj->Set_Damage(3);
}
