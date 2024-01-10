#include "stdafx.h"
#include "Tile.h"
#include "BitmapMgr.h"
#include "ScrollMgr.h"

void CTile::Update_Rect()
{
	m_tRect.left = long(m_tInfo.fX - m_tInfo.fCX * 0.5f);
	m_tRect.top = long(m_tInfo.fY - m_tInfo.fCY * 0.5f);
	m_tRect.right = long(m_tInfo.fX + m_tInfo.fCX * 0.5f);
	m_tRect.bottom = long(m_tInfo.fY + m_tInfo.fCY * 0.5f);
}

void CTile::Move_Frame()
{
}

CTile::CTile()
	:m_iDrawX(0), m_iDrawY(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(m_szImgKey, sizeof(m_szImgKey));
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
}

int CTile::Update()
{
	Update_Rect();
	return 0;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY),
		TILECX, TILECY, hMemDC,
		(TILECX + 4) * m_iDrawX + 2, (TILECY + 4) * m_iDrawY + 2,
		TILECX, TILECY, RGB(255, 0, 144));

}

void CTile::Release()
{
}
