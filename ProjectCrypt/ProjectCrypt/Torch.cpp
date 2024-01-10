#include "stdafx.h"
#include "Torch.h"
#include "BitmapMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"

CTorch::CTorch()
{
}

CTorch::~CTorch()
{
	Release();
}

void CTorch::Initialize()
{
	m_tInfo.fCX = 24.f;
	m_tInfo.fCY = 52.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();

	m_tState.iHP = 1;

	m_eID = OBJ_TORCH;
}

int CTorch::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CTorch::Late_Update()
{
	if (m_tState.iHP <= 0)
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		m_bDead = true;
		return;
	}
	__super::Move_Frame(m_tFrame);
}

void CTorch::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 40),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		int(m_tInfo.fCX * m_tFrame.iFrameStart), int(m_tInfo.fCY * m_tFrame.iMotion),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}

void CTorch::Release()
{
}

bool CTorch::Collision()
{
	return false;
}
