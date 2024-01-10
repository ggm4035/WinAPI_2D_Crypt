#include "stdafx.h"
#include "Money.h"
#include "TileMgr.h"
#include "ScrollMgr.h"
#include "BitmapMgr.h"

CMoney::CMoney(const int& _iMoney)
{
	m_tState.iMoney = _iMoney;
}

CMoney::~CMoney()
{
	Release();
}

bool CMoney::Collision()
{
	return false;
}

void CMoney::Initialize()
{
	lstrcpy(m_szImgKey, L"gold_small");
	m_eID = OBJ_MONEY;
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;
}

int CMoney::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CMoney::Late_Update()
{
}

void CMoney::Render(HDC _hDC)
{
	float fScrollX = SCROLL_MGR->Get_ScrollX();
	float fScrollY = SCROLL_MGR->Get_ScrollY();

	HDC hMemDC = BITMAP_MGR->Find_Img(m_szImgKey);

	GdiTransparentBlt(_hDC, int(m_tRect.left + fScrollX), int(m_tRect.top + fScrollY - 10),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, int(m_tInfo.fCY * m_tFrame.iMotion),
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}

void CMoney::Release()
{
}
