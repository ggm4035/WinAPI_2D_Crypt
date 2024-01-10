#include "stdafx.h"
#include "Trap.h"
#include "TileMgr.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"

CTrap::CTrap()
{
}

CTrap::~CTrap()
{
	Release();
}

bool CTrap::Collision()
{
	return false;
}

void CTrap::Initialize()
{
	m_tInfo.fCX = 28.f;
	m_tInfo.fCY = 32.f;

	m_tState.iHP = 3;

	m_eID = OBJ_TRAP;
}

int CTrap::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CTrap::Late_Update()
{
	if (m_tState.iHP <= 0)
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		m_bDead = true;
		return;
	}
}

void CTrap::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 10),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		int(m_tInfo.fCX * m_tFrame.iFrameStart), int(m_tInfo.fCY * m_tFrame.iMotion),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}

void CTrap::Release()
{
}

void CTrap::Move_Object(CObj * _pObj)
{
	//if(!lstrcmp(m_szImgKey, L""))
}
