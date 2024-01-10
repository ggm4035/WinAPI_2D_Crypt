#include "stdafx.h"
#include "Beat.h"
#include "TimeMgr.h"
#include "BitmapMgr.h"
#include "ObjMgr.h"

void CBeat::Update_Rect()
{
	m_tRect.left = long(m_tInfo.fX - m_tInfo.fCX * 0.5f);
	m_tRect.top = long(m_tInfo.fY - m_tInfo.fCY * 0.5f);
	m_tRect.right = long(m_tInfo.fX + m_tInfo.fCX * 0.5f);
	m_tRect.bottom = long(m_tInfo.fY + m_tInfo.fCY * 0.5f);
}

CBeat::CBeat()
	:m_fSpeed(0.f), m_bHiting(false), m_bDead(false)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CBeat::~CBeat()
{
	Release();
}

void CBeat::Initialize()
{
	m_tInfo = {-100.f, 530.f, 12.f, 64.f};
	m_fSpeed = 5.f;
	m_dwTime = GetTickCount();
}

int CBeat::Update()
{
	if (m_bDead)
		return BEAT_DEAD;

	m_tInfo.fX += m_fSpeed;

	Update_Rect();

	if (m_bHiting)
		return BEAT_HITTING;

	return BEAT_NOEVENT;
}

void CBeat::Late_Update()
{
	if (m_bDead)
		return;

	if (395 < m_tInfo.fX)
		m_fSpeed = 0.5f;

	if (300 < m_tInfo.fX)
		m_bHiting = true;

	if (405 < m_tInfo.fX)
	{
		m_bHiting = false;
		m_bDead = true;
		// 보스가 화나요
		if(OBJ_MGR->Find_Monster(BOSS))
			OBJ_MGR->Find_Monster(BOSS)->Set_Wrong_Beat(true);
	}
}

void CBeat::Render(HDC _hDC)
{
	HDC hMemDC = BITMAP_MGR->Find_Img(L"TEMP_beat_marker");

	GdiTransparentBlt(_hDC, int(m_tRect.left), int(m_tRect.top),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, 0,
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));

	GdiTransparentBlt(_hDC, 800 - int(m_tRect.left), int(m_tRect.top),
		int(m_tInfo.fCX), int(m_tInfo.fCY), hMemDC,
		0, 0,
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(255, 0, 144));
}

void CBeat::Release()
{
}
