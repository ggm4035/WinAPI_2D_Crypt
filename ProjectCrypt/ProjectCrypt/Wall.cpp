#include "stdafx.h"
#include "Wall.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"
#include "TileMgr.h"

CWall::CWall()
	:m_eWallID(WALL_END), m_bShovel(false), m_dwTime(GetTickCount()), m_pShovel(nullptr)
{
}

CWall::~CWall()
{
	Release();
}

void CWall::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY * 2;
	m_eID = OBJ_WALL;

	switch (m_eWallID)
	{
	case WALL_BREAK:
		m_tState.iHP = 1;
		break;

	case WALL_BOOM_BREAK:
		m_tState.iHP = 2;
		break;

	case WALL_UNBREAK:
		m_tState.iHP = 100;

	case WALL_DOOR:
		m_tState.iHP = 1;
		break;
	}

	if (!lstrcmp(m_szImgKey, L"wall_dirt_crypt"))
		m_tFrame.iFrameStart = rand() % 16;

	else if (!lstrcmp(m_szImgKey, L"boss_wall"))
		m_tFrame.iFrameStart = rand() % 5;
}

int CWall::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CWall::Late_Update()
{
	if (m_tState.iHP <= 0)
	{
		TILE_MGR->Pop_Object(m_tInfo.fX, m_tInfo.fY);
		m_bDead = true;
	}
}

void CWall::Render(HDC _hDC)
{
	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);	// 키 값에 따라 금, 흙, 돌 등 변하게

	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY),
		int(48), int(77), hMemDC,
		TILECX * m_tFrame.iFrameStart, 0,
		TILECX, 77, RGB(255, 0, 144));

	if (m_bShovel)
		Render_Shovel(_hDC);
}

void CWall::Release()
{
}

bool CWall::Collision()
{
	return false;
}

void CWall::Render_Shovel(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hEffectDC = BITMAP_MGR->Find_Img(m_pShovel);

	GdiTransparentBlt(_hDC, 
		int(m_tRect.left + fScrollX),
		int(m_tRect.top + fScrollY),
		48, 48, hEffectDC,
		0, 0,
		48, 48, RGB(255, 0, 144));

	if (m_dwTime + 100 < GetTickCount())
		m_bShovel = false;
}
